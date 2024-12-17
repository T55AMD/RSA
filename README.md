# 基于 GMP 库的 RSA 实现
## 项目简介
本项目实现了 RSA 加密算法，使用了 GNU Multiple Precision Arithmetic Library (GMP) 来处理大整数运算。RSA 是一种非对称加密算法，广泛应用于安全通信、数字签名等领域。通过本项目，用户可以生成公钥和私钥对，并进行加密和解密操作。

## 环境依赖
本项目在 Visual Studio 2022 下进行开发，因此不保证在其他环境下能正常运行。为了确保项目能够顺利编译和运行，请确保你已经安装了以下依赖项：        
Visual Studio 2022：用于编译和调试 C++ 代码。   
本项目下已经有作者下载的完整GMP库，因此不需要再次用户自行安装。 
## 使用方法 

### 加密 
加密文件应当只包含大写字母和空格，作者不保证其他输入会发生什么。    
用户首先选择进行加密（E），然后输入希望以多少个字母为一组，文章末尾如果不足，会被空格填满。     
此时会生成公钥对和私钥对，用户可在目录下查看。  
用户此时可以把希望加密的文件放到$Project2$目录下，输入希望加密的文件名称,然后程序会输出加密后的文件(名称为encryptoed.txt)。
如果用户想要对刚刚的文件进行解密，请查看解密步骤。  

### 解密

用户应选择解密(D)，然后输入和刚刚**相同**的分组长度。   
此处默认输入的文件名是"encryptoed.txt"。    
输入此文件名后，用户即可在屏幕上查看解密出的内容。  



## 已知的bug
如果在输入长度时不输入数字，而是输入文件名称，程序会直接卡死。请重启该程序。