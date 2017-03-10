#include <stdio.h>  // 标准输入输出函数
#include <stdlib.h>  // 标准库函数
#include <string.h>  // 字符串处理函数
#include <ctype.h>  // 字符操作函数
#include <conio.h>  // 控制台输入输出函数，虽然不是标准头文件，但是大部分编译器都支持

/**
 * 在文件开头声明函数原型，将函数定义放在main()函数后面
 * 这是一个优雅的编程习惯
**/
void printMenu(void);
int checkPwd(int type);
int encryptFile(char *sourcefile, char *secretKey,char *targetFile);
char redo(void);

/**
 * password 被main()和checkPwd()两个函数使用
 * 必须定义为全局变量，即在main()函数外部定义
**/
int  password=123456;  // 软件初始密码

int main(){
	/**
	 * 不像 password 变量，下面这些变量只在main()函数内部使用
	 * 所以可以定义为局部变量，即定义在mian()函数内部
	**/
	int newpwd, newpwda;  // 更改密码时，用户两次输入的新密码
	char sourcefile[30],  // 加密/解密的文件名
		targetFile[30],  // 解密/加密后要保存的文件名
		secretKey[21],  // 文件加密的密钥
		action; // 要进行的操作
	
	// 操作前先校验密码
	if(!checkPwd(1)){
		printf("抱歉，3次输入密码错误。按任意键退出程序...");
		getch();
		exit(1);
	}

	// 密码校验成功，显示程序菜单，执行各种操作
	while(1){
		system("cls");  // 清屏
		printMenu();  // 显示菜单
		scanf("%c",&action);  // 输入要执行的操作
		fflush(stdin);  // 清空stdin缓冲区
		system("cls");

		switch(action){
			// 退出系统
			case 'z':
				exit(0);
				break;

			// 文件加密
			case 'a':
				while(1){
					printf("输入要加密的文件名(含路径)：");
					scanf("%s", sourcefile);
					printf("输入密钥：");  //密钥是用户自己定义的，可以随意给需要加密的文件添加密钥
					scanf("%s", secretKey);
					printf("加密后的文件名(含路径)：");  //给加密后的文件命名，并保存
					scanf("%s",targetFile);
					if( encryptFile(sourcefile, secretKey, targetFile) ){
						printf("恭喜你，文件[%s]加密成功，保存在[%s]。\n", sourcefile, targetFile);
					}

					if(redo() == 'a'){  // 继续加密文件
						system("cls");
					}else{  // 回到主菜单
						break;
					}
				}
				break;

			// 文件解密
			case 'b':
				while(1){
					printf("输入要解密的文件名(含路径)：");
					scanf("%s",sourcefile);
					printf("输入密钥：");
					scanf("%s",secretKey);
					printf("解密后的文件名(含路径)：");  //对解密的文件系统又可以提供保存路径
					scanf("%s", targetFile);
					if( encryptFile(sourcefile, secretKey, targetFile) ){
						printf("恭喜你，文件[%s]解密成功，保存在[%s]。\n", sourcefile, targetFile);
					}

					if(redo() == 'a'){  // 继续解密文件
						system("cls");
					}else{
						break;  // 回到主菜单
					}
				}
				break;

			// 修改密码
			case 'c':
				while(1){
					if(!checkPwd(2)){
						printf("抱歉，3次输入密码错误。按任意键退出程序...");
						getch();
						exit(0);
					}
				
					printf("请输入新密码：");
					scanf("%d",&newpwd);
					printf("请再次输入新密码：");
					scanf("%d",&newpwda);
					if(newpwd==newpwda){  // 两次输入密码相同才成功
						password = newpwd;
						printf("恭喜你，修改密码成功！\n");
					}else{
						printf("两次输入密码不相同，修改失败！\n");
					}

					if(redo() == 'a'){  // 继续修改密码
						system("cls");
					}else{  // 回到主菜单
						break;
					}
				}
				break;

			default:
				printf("没有相应的菜单！按任意键回到主菜单...");
				getch();
		}
	}

	return EXIT_SUCCESS;
}

/**
 * 显示程序菜单
**/
void printMenu(){
	printf("***************** 文本加密解密软件 *****************\n");
	printf("*                                                  *\n");
	printf("*      **************************************      *\n");
	printf("*      *                                    *      *\n");
	printf("*      *  版权所有：C语言中文网             *      *\n");
	printf("*      *  作者：棍哥                        *      *\n");
	printf("*      *  PS：不要问我为什么叫棍哥          *      *\n");
	printf("*      *                                    *      *\n");
	printf("*      **************************************      *\n");
	printf("*                                                  *\n");
	printf("*                                                  *\n");
	printf("*      请从下面的菜单中选择你要进行的操作：        *\n");
	printf("*      a. 文件加密                                 *\n");
	printf("*      b. 文件解密                                 *\n");
	printf("*      c. 更改密码                                 *\n");
	printf("*      z. 退出系统                                 *\n");
	printf("*                                                  *\n");
	printf("****************************************************\n");
}

/**
 * 校验密码
 *
 * @param   type    校验类型（区别不同的校验类型主要是为了输出不同的提示语句）
     1：进入程序时校验密码
     2：修改密码时校验密码
 *
 * @return  教程成功或失败的数字表示
     0：校验失败
     1：校验成功
**/
int checkPwd(int type){
	int pwd;  //用户输入的密码
	int trytimes;  // 用户尝试输入密码的次数
	if(type==1){
		printf("程序设置了密码，验证通过后才能使用。请输入6位数字密码：");
	}else{
		printf("重置密码前必须验证原密码。请输入6位数字原密码：");
	}
	// 可以输入三次
	for(trytimes=1; trytimes<=3; trytimes++){
		scanf("%d",&pwd);
		fflush(stdin);
		if(pwd==password){
			return 1;
			break;
		}else if(trytimes==3){
			return 0;
		}else{
			printf("抱歉，密码错误，您还有%d次机会：", 3-trytimes);
		}
	}

	// 没有最后的return语句，并不妨碍程序的运行
	// 但是某些编译器会产生”不是所有的流程控制结构都有返回值“的警告
	return 0;
}

/**
 * 加密/解密文件
 *
 * @param   sourcefile    要加密/解密的文件名
 * @param   secretKey     密钥
 * @param   targetFile    加密/解密后要保存的文件名
 *
 * @return  加密成功或失败的数字表示
     0：加密失败
     1：加密成功
**/
int encryptFile(char *sourcefile, char *secretKey, char *targetFile){
	FILE *fpSource, *fpTarget;  // 要打开的文件的指针
	char buffer[21];  // 缓冲区，用于存放从文件读取的数据
	int readCount,  // 每次从文件中读取的字节数
		keyLen = strlen(secretKey),  // 密钥的长度
		i;  // 循环次数

	// 以二进制方式读取/写入文件
	fpSource = fopen(sourcefile, "rb");
	if(fpSource==NULL){
		printf("文件[%s]打开失败，请检查文件路径和名称是否输入正确！\n", sourcefile);
		return 0;
	}
	fpTarget = fopen(targetFile, "wb");
	if(fpTarget==NULL){
		printf("文件[%s]创建/写入失败！请检查文件路径和名称是否输入正确！\n", fpTarget);
		return 0;
	}

	// 不断地从文件中读取 keyLen 长度的数据，保存到buffer，直到文件结束
	while( (readCount=fread(buffer, 1, keyLen, fpSource)) > 0 ){
		// 对buffer中的数据逐字节进行异或运算
		for(i=0; i<readCount; i++){
			buffer[i] ^= secretKey[i];
		}
		// 将buffer中的数据写入文件
		fwrite(buffer, 1, readCount, fpTarget);
	}

	fclose(fpSource);
	fclose(fpTarget);

	return 1;
}

/**
 * 当前操作完成，要进行的下一次操作（次要菜单）
 *
 * @return  下一次操作的字母表示
     a：回到主菜单
     b：继续当前操作
**/
char redo(){
	char action;

	printf("\n接下来你希望：\n");
	printf("a. 继续当前操作\n");
	printf("b. 回到主菜单\n");

	// 不停的等待用户输入，直到输入正确
	while(1){
		fflush(stdin);
		scanf("%c", &action);
		fflush(stdin);
		if(action!='a' && action!='b'){
			printf("没有相应的菜单，请重新选择。\n");
		}else{
			return action;
			break;
		}
	}
}