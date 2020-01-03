# PoissonDeformation
**作者:** [Zhengjun-Du], 有任何疑问请联系：duzjqhu@aliyun.com

参考论文: Yu Y , Zhou K , Xu D , et al. Mesh editing with poisson-based gradient field manipulation[J]. ACM Transactions on Graphics, 2004, 23(3):644.

## 1. 编译环境及第三方库
* Win10， VS2012或跟高版本， X64编译
* 需要用到的第三方库包括：**OpenMesh**， **OpenGL**， **Eigen**， 第三方库已经包括在*libs*文件夹，第三方库已经配置好，你不需要配置环境。


## 2. 核心代码
* 核心代码位于: /MeshDeformation/MeshDeform/PoissonDeform/PoissonDeformation.cpp (计算梯度和散度)
* 以及：  /MeshDeformation/Utility/MeshLaplacianSolver.cpp （构建拉普拉斯矩阵）

## 3.如何使用
项目提供了一个长方体测试模型以及控制点位于*test_model*文件夹
* 1）点击**打开**按钮，选中*test_model*文件夹中的*cuboid.obj*模型
* 2）点击上方**泊松变形**
* 3）点击左侧**导入边界**，选中选中*test_model*文件夹中的*cuboid.txt*文件
* 4）选中长方体底部部分区域，点击左侧**设定固定区域**
* 5）点击左侧**计算拉普拉斯矩阵**
* 6）目前提供三种交互：A. 绕Y轴的旋转(鼠标中键 + Ctrl),  B. 绕X轴的旋转(鼠标中键 + Shift)  C. 平移(Ctrl+鼠标右键)
* 7）点击左侧**泊松变形**

## 4.论文讲解
* [https://blog.csdn.net/u011426016/article/details/103727265](https://blog.csdn.net/u011426016/article/details/103727265) 
