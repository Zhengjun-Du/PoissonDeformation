# My implementation of  Mesh editing with poisson-based gradient field manipulation
**My email:** duzjqhu@aliyun.com, any questions, please contact me.  

Reference: Yu Y , Zhou K , Xu D , et al. Mesh editing with poisson-based gradient field manipulation[J]. ACM Transactions on Graphics, 2004, 23(3):644.

## 1. Platform and thirdparty libs
* Win10  
* VS2012 or higher version, X64  
* OpenMesh  
* OpenGL    
* Eigen  

## 2. Core code
* locate in: /MeshDeformation/MeshDeform/PoissonDeform/PoissonDeformation.cpp (calculate gradient and divergence)  
* /MeshDeformation/Utility/MeshLaplacianSolver.cpp （build laplacian matrix）  

## 3. Usage
The test model locate in "/test_model" directory, include the model and control points.  
* 1）click **打开** button，select cuboid.obj in "/test_model"
* 2）click **泊松变形**
* 3）click **导入边界** on the left pannel, select cuboid.txt in "/test_model"
* 4）select the bottom region of the cuboid，click **设定固定区域** on the left pannel  
* 5）click **计算拉普拉斯矩阵** on the left pannel
* 6）provide 3 interactions：A. rotate with Y axis(mid button of mouse + Ctrl),  B. rotate with X-axis(mid button of mouse + Shift)  C. translate(Ctrl+ mid button of mouse)
* 7）click **泊松变形** 

## 4. Paper analysis in Chinese
* [https://blog.csdn.net/u011426016/article/details/103727265](https://blog.csdn.net/u011426016/article/details/103727265) 
