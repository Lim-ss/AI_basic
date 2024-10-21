#from sklearn.datasets import load_wine
import numpy as np 
import matplotlib.pyplot as plt
from gensim.models import KeyedVectors

def linear_kernel(X, Y):
    return np.dot(X, Y.T)

def rbf_kernel(X, Y, gamma=0.1):
    sq_dists = -2 * np.dot(X, Y.T) + np.sum(Y**2, axis=1) + np.sum(X**2, axis=1)[:, np.newaxis]
    return np.exp(-gamma * sq_dists)

def get_kernel_function(kernel: str):
    # TODO: implement different kernel functions 
    if kernel == "rbf":
        return rbf_kernel
    elif kernel == "linear":
        return linear_kernel
    else:
        raise ValueError(f"Unsupported kernel: {kernel}")

class PCA:
    def __init__(self, n_components: int = 2, kernel: str = "rbf") -> None:
        self.n_components = n_components
        self.kernel_f = get_kernel_function(kernel)
        self.components = None # 投影矩阵

    def fit(self, X:np.ndarray):
        # X: [n_samples, n_features]
        # TODO: implement PCA algorithm

        #计算核矩阵
        K = self.kernel_f(X, X)

        # 中心化核矩阵
        N = K.shape[0]
        one_n = np.ones((N, N)) / N
        K_centered = K - one_n @ K - K @ one_n + one_n @ K @ one_n

        # # 数据中心化
        # X_centered = X - np.mean(X, axis=0)
        # # 计算协方差矩阵
        # covariance_matrix = np.cov(X_centered, rowvar=False)
        # # 进行特征值分解
        # eigenvalues, eigenvectors = np.linalg.eigh(covariance_matrix)

        # 进行特征值分解
        eigenvalues, eigenvectors = np.linalg.eigh(K_centered)

        # 按特征值降序排列特征向量
        sorted_idx = np.argsort(eigenvalues)[::-1]
        eigenvectors = eigenvectors[:, sorted_idx]
        
        # 选择前 n_components 个特征向量
        self.components = eigenvectors[:, :self.n_components]

        return self

    # def transform(self, X:np.ndarray):
    #     # X: [n_samples, n_features]
    #     # TODO: transform the data to low dimension
    
    #     # 数据中心化
    #     X_centered = X - np.mean(X, axis=0)
    #     # 将数据投影到主成分上
    #     X_reduced = np.dot(X_centered, self.components)
    #     return X_reduced
    
    def transform(self, X: np.ndarray):
        # X: [n_samples, n_features]
        # TODO: transform the data to low dimension

        # 核矩阵
        K = self.kernel_f(X, X)
        # 中心化核矩阵
        N = K.shape[0]
        one_n = np.ones((N, N)) / N
        K_centered = K - one_n @ K - K @ one_n + one_n @ K @ one_n

        return np.dot(K_centered, self.components)

class KMeans:
    def __init__(self, n_clusters:int=3, max_iter:int=10) -> None:
        self.n_clusters = n_clusters
        self.max_iter = max_iter
        self.centers = None
        self.labels = None

    # 随机初始化中心点
    def initialize_centers(self, points)->None:
        # points: (n_samples, n_dims,)
        n, d = points.shape
        self.centers = np.zeros((self.n_clusters, d))
        for i in range(self.n_clusters):
            # use more random points to initialize centers, make kmeans more stable
            random_index = np.random.choice(n, size=10, replace=False)
            self.centers[i] = points[random_index].mean(axis=0)
    
    # 将每个点分配到最近的中心(这里不直接对self.labels进行修改而是返回，是因为预测时也需要用到这段代码，避免预测时破坏聚类的结果)
    def assign_points(self, points):
        # points: (n_samples, n_dims,)
        # return labels: (n_samples, )
        # TODO: Compute the distance between each point and each center and Assign each point to the closest center
        n_samples = points.shape[0]
        labels = np.zeros(n_samples)
        for i in range(n_samples):
            distances = np.linalg.norm(points[i] - self.centers, axis=1)
            labels[i] = np.argmin(distances)
        return labels

    # 基于新的点分配更新中心点
    def update_centers(self, points):
        # points: (n_samples, n_dims,)
        # TODO: Update the centers based on the new assignment of points
        for i in range(self.n_clusters):
            points_in_cluster = points[self.labels == i]
            if len(points_in_cluster) > 0:
                self.centers[i] = points_in_cluster.mean(axis=0)

    # k-means clustering
    def fit(self, points):
        # points: (n_samples, n_dims,)
        # TODO: Implement k-means clustering

        # 初始化中心点
        self.initialize_centers(points)
        
        for _ in range(self.max_iter):
            # 将点分配到最近的簇
            old_labels = self.labels.copy() if self.labels is not None else None
            self.labels = self.assign_points(points)
            
            # 更新中心点
            self.update_centers(points)
            
            # 检查是否收敛
            if np.array_equal(old_labels, self.labels):
                break

        return self

    def predict(self, points):
        return self.assign_points(points)

    # Predict the closest cluster each sample in X belongs to
    def predict(self, points):
        # points: (n_samples, n_dims,)
        # return labels: (n_samples, )
        return self.assign_points(points)
    
def load_data():
    words = [
        'computer', 'laptop', 'minicomputers', 'PC', 'software', 'Macbook',
        'king', 'queen', 'monarch','prince', 'ruler','princes', 'kingdom', 'royal',
        'man', 'woman', 'boy', 'teenager', 'girl', 'robber','guy','person','gentleman',
        'banana', 'pineapple','mango','papaya','coconut','potato','melon',
        'shanghai','HongKong','chinese','Xiamen','beijing','Guilin',
        'disease', 'infection', 'cancer', 'illness', 
        'twitter', 'facebook', 'chat', 'hashtag', 'link', 'internet',
    ]
    w2v = KeyedVectors.load_word2vec_format('./../data/GoogleNews-vectors-negative300.bin', binary = True)
    vectors = []
    for w in words:
        vectors.append(w2v[w].reshape(1, 300))
    vectors = np.concatenate(vectors, axis=0)
    return words, vectors

if __name__=='__main__':
    words, data = load_data()
    pca = PCA(n_components=2,kernel="rbf",).fit(data)
    data_pca = pca.transform(data)

    kmeans = KMeans(n_clusters=7).fit(data_pca)
    clusters = kmeans.predict(data_pca)

    # 绘制数据
    plt.figure()
    plt.scatter(data_pca[:, 0], data_pca[:, 1], c=clusters)
    for i in range(len(words)):
        plt.annotate(words[i], data_pca[i, :]) 
    plt.title("PB21050974")
    plt.savefig("PCA_KMeans.png")