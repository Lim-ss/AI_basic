#from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import numpy as np 
import pandas as pd 
from collections import Counter
#import pydotplus
from graphviz import Digraph

# 决策树可视化
def visualize_tree(tree, feature_names):
    def add_nodes_edges(dot, node, parent=None, edge_label=""):
        if node.label is not None:
            dot.node(str(id(node)), f"Label: {node.label}")
        else:
            if node.split_value is None:
                dot.node(str(id(node)), f"{feature_names[node.feature]} == {node.value}")
            else:
                dot.node(str(id(node)), f"{feature_names[node.feature]} <= {node.split_value}")

        if parent is not None:
            dot.edge(str(id(parent)), str(id(node)), label=edge_label)

        for key, child in node.branches.items():
            if node.split_value is None:
                add_nodes_edges(dot, child, node, f"== {key}")
            else:
                if key == 'left':
                    add_nodes_edges(dot, child, node, f"<= {node.split_value}")
                else:
                    add_nodes_edges(dot, child, node, f"> {node.split_value}")

    dot = Digraph()
    add_nodes_edges(dot, tree)
    return dot

# 计算信息熵
def entropy(y):
    counter = Counter(y)
    total = len(y)
    Ent = -sum((count / total) * np.log2(count / total) for count in counter.values())
    return Ent

# 对于某个feature计算信息增益，根据split_value决定是离散值还是连续值
def information_gain(X, y, feature, split_value=None):
    total_entropy = entropy(y)
    
    if split_value is None:
        values, counts = np.unique(X[:, feature], return_counts=True)
        weighted_entropy = sum((counts[i] / np.sum(counts)) * entropy(y[X[:, feature] == v]) for i, v in enumerate(values))
    else:
        left_mask = X[:, feature] <= split_value
        right_mask = X[:, feature] > split_value
        left_entropy = entropy(y[left_mask])
        right_entropy = entropy(y[right_mask])
        weighted_entropy = (sum(left_mask) / len(y)) * left_entropy + (sum(right_mask) / len(y)) * right_entropy
    
    return total_entropy - weighted_entropy

# 求连续属性的最优划分
def best_split(X, y, feature):
    unique_values = np.unique(X[:, feature])
    if len(unique_values) == 1:
        return None, None

    best_gain = -1
    best_split_value = None

    for value in unique_values:
        gain = information_gain(X, y, feature, split_value=value)
        if gain > best_gain:
            best_gain = gain
            best_split_value = value

    return best_gain, best_split_value

# 决策树节点类
# feature  当前节点用于划分数据的特征索引
# value    表示节点在划分时所依据的特征值（仅当feature是离散值时有意义）
# split_value 表示节点在划分时所依据的划分值（仅当feature是连续值时有意义）
# branches 子节点分支
# label    当前叶节点的类别标签（仅当是叶节点时有意义）
class Node:
    def __init__(self, feature=None, value=None, split_value=None, branches=None, label=None):
        self.feature = feature
        self.value = value
        self.split_value = split_value
        self.branches = branches or {}
        self.label = label

# 准确率计算
def accuracy(y_true, y_pred):
    return sum(y_true == y_pred) / len(y_true)

# 决策树模型
class DecisionTreeClassifier:
    # 在这里可以限定决策树的深度，如果不填，默认无限深度
    def __init__(self, max_depth=None):
        self.max_depth = max_depth
        self.tree = None

    # 在这里指定哪些是连续值
    def fit(self, X, y, continuous_features=[]):
        # X: [n_samples_train, n_features], 
        # y: [n_samples_train, ],
        # TODO: implement decision tree algorithm to train the model
        self.continuous_features = continuous_features
        self.tree = self._build_tree(X, y)

    def _build_tree(self, X, y, depth=0):
        # 所有样本类别相同，无需再分类
        if len(np.unique(y)) == 1:
            return Node(label=y[0])

        # 属性集为空，或者树到达最大深度，无法再分类
        if X.shape[1] == 0 or (self.max_depth is not None and depth >= self.max_depth):
            return Node(label=Counter(y).most_common(1)[0][0])

        # 计算最大信息增益
        best_gain = -1
        best_feature = None
        best_split_value = None
        for feature in range(X.shape[1]):
            # 对于连续值和离散值处理方式不同
            if feature in self.continuous_features:
                gain, split_value = best_split(X, y, feature)
                if gain is not None and gain > best_gain:
                    best_gain = gain
                    best_feature = feature
                    best_split_value = split_value
            else:
                gain = information_gain(X, y, feature)
                if gain > best_gain:
                    best_gain = gain
                    best_feature = feature
                    best_split_value = None

        # 最大信息增益为0，说明所有样本的属性相同，无法再分类
        if best_gain == 0:
            return Node(label=Counter(y).most_common(1)[0][0])

        # 最大信息增益不为0，可以进行分类，递归生成子树
        node = Node(feature=best_feature, split_value=best_split_value)
        if best_split_value is None: 
            # 离散值
            values = np.unique(X[:, best_feature])
            for value in values:
                idx = X[:, best_feature] == value
                child_node = self._build_tree(X[idx], y[idx], depth + 1)
                node.branches[value] = child_node
                # 当没有匹配的分支时，预测值取当前节点概率最大的类别
                node.branches[None] = Node(label=Counter(y).most_common(1)[0][0])
        else: 
            # 连续值
            left_mask = X[:, best_feature] <= best_split_value
            right_mask = X[:, best_feature] > best_split_value
            node.branches['left'] = self._build_tree(X[left_mask], y[left_mask], depth + 1)
            node.branches['right'] = self._build_tree(X[right_mask], y[right_mask], depth + 1)

        return node

    # 预测一组样本
    def predict(self, X):
        # X: [n_samples_test, n_features]
        # return: y: [n_samples_test]
        # TODO:
        y = np.array([self._predict(inputs) for inputs in X])
        return y

    # 预测一个样本
    def _predict(self, inputs):
        node = self.tree
        while node.branches:
            if node.split_value is None:
                # 离散值
                value = inputs[node.feature]
                if node.branches.get(value) is None:
                    #print("没有匹配的分支") 
                    node = node.branches.get(None)
                else:
                    node = node.branches.get(value)
            else:
                # 连续值
                if inputs[node.feature] <= node.split_value:
                    node = node.branches['left']
                else:
                    node = node.branches['right']
        return node.label

# 加载数据以及预处理：
# 读取 CSV 文件。
# 将特征分为连续特征和离散特征。
# 对离散特征进行标签编码，将字符串值转换为数值。
# 将数据集分为训练集和测试集
def load_data(datapath:str='./data/ObesityDataSet_raw_and_data_sinthetic.csv'):
    df = pd.read_csv(datapath)    
    continue_features = ['Age', 'Height', 'Weight', 'FCVC', 'NCP', 'CH2O', 'FAF', 'TUE']
    discrete_features = ['Gender', 'CALC', 'FAVC', 'SCC', 'SMOKE', 'family_history_with_overweight', 'CAEC', 'MTRANS']
    discrete_features_size = {'Gender':2, 'CALC':4, 'FAVC':2, 'SCC':2, 'SMOKE':2, 'family_history_with_overweight':2, 'CAEC':4, 'MTRANS':5}

    X, y = df.iloc[:, :-1], df.iloc[:, -1]
    # encode discrete str to number, eg. male&female to 0&1
    labelencoder = LabelEncoder()
    for col in discrete_features:
        X[col] = labelencoder.fit(X[col]).transform(X[col])
    y = labelencoder.fit(y).fit_transform(y)
    
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

    return X_train, X_test, y_train, y_test

# 主函数
if __name__=="__main__":
    X_train, X_test, y_train, y_test = load_data('./../data/ObesityDataSet_raw_and_data_sinthetic.csv')

    # 连续值的索引列表
    continuous_features = []
    # 将字符串索引变成数字索引，这个操作需要在X_train转换为numpy数组之前
    for column in ['Age', 'Height', 'Weight', 'FCVC', 'NCP', 'CH2O', 'FAF', 'TUE']:
        index = X_train.columns.get_loc(column)
        continuous_features.append(index)

    # 提取列名
    feature_names = X_train.columns.tolist()

    # 将X由Pandas DataFrame转换为numpy数组
    X_train = X_train.to_numpy().round(2)
    X_test = X_test.to_numpy().round(2)

    clf = DecisionTreeClassifier(10)
    clf.__init__()
    clf.fit(X_train, y_train,continuous_features)

    y_pred = clf.predict(X_test)
    print("测试集预测准确率：",accuracy(y_test, y_pred))
    y_pred = clf.predict(X_train)
    print("训练集预测准确率：",accuracy(y_train, y_pred))
    
    # 生成决策树图片（需要安装graphviz并配置环境变量）
    dot = visualize_tree(clf.tree, feature_names)
    dot.render("decision_tree", format="png", cleanup=True)