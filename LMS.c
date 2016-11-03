#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <windows.h>

//全局变量
int check_login_success = 0;//在欢迎标题显示用户名称时会用到
char user_number[12];//在欢迎标题显示用户账号名时会用到
struct sysuser //定义sysuser结构体 
{
    char usernum[12]; //学号和手机号都是11位的
					//【在校学生】直接通过学号+默认密码123登录，【外来人员】则通过手机号注册 
                    //而且学号和手机号都是唯一的，所以学号和手机号都能作为sysuser的主码
                    //由于c语言规定在存放字符串的数组里要以字符串结束符来结束，也就是‘\0’
					//所以数组usernum[]的长度为字符总个数11加上一个结束符‘\0’也就是12 
    char password[8];
    struct sysuser *next; //一个指向sysuser对象的指针!!!!!一定要自己声明这个next指针！！不要忘记声明！！
						 //只有C#这种懒虫语言才会默默帮你把next指针声明的好嘛，C还是要自己声明的 
	//为了方便测试功能，我注册了个账号密码都为1的用户。。。 
};
struct book //定义book结构体
{
    int number;     //编号
    char name[30];  //书名
    char category[10];  //类别
    int lent;       //是否借出（已借为1，未借为0）
    int year;       //借出时间——年
    int month;      //借出时间——月
    int day;        //借出时间——日
    struct book *next; //一个指向book类型对象的指针 
};


//欢迎标题
void print_main_title()
{
    time_t time_login;
    struct tm *p;
    time(&time_login);
    p = gmtime(&time_login);

    if(check_login_success == 0)
    {
        printf("--------------------------------------------------------------------------------");
        printf("欢迎您，亲爱的用户！\n");
        printf("                                                                                ");
        printf("                           欢迎使用图书管理系统                                 ");
        printf("                                                                                ");
        printf("                        现在时间：%d年%d月%d日 %d:%d:%d\n", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
        printf("                                                                                ");
        printf("                                                                                ");
        printf("--------------------------------------------------------------------------------");
    }
    else
    {
        printf("--------------------------------------------------------------------------------");
        printf("欢迎您，%s！\n",user_number);
        printf("                                                                                ");
        printf("                           欢迎使用图书管理系统v2.5                             ");
        printf("                                                                                ");
        printf("                        现在时间：%d年%d月%d日 %d:%d:%d\n", 1900 + p->tm_year, p->tm_mon, p->tm_mday, 8 + p->tm_hour, p->tm_min, p->tm_sec);
        printf("                                                                                ");
        printf("                                                                                ");
        printf("--------------------------------------------------------------------------------");
    }

}


//定义check_usernum()函数(create_user()时调用，检测用户账号是否被注册过，目的是为了保证usernum作为主码的唯一性) 
int check_usernum(struct sysuser *user)
{
	int check=0;
	
	char usrno[12];
	strcpy(usrno, user -> usernum);
	
	char usrno_f[12];
	FILE *fp;
	fp = fopen("user.txt", "r+");
	if (fp == NULL) fopen("user.txt", "w");
	while(fscanf(fp, "%s%*s", usrno_f) != EOF)
    {	
        if(strcmp(usrno, usrno_f) == 0)
        {
            check = 1; 
            break;
        }
    }
    if(fclose(fp))
    {
        printf("Can not close the file!\n");
        exit(0);
    }
	
	return check; //被注册过返回1，没被注册过返回0 
} 
//注册
int create_user()
{	
    struct sysuser su;//创建一个su变量，用sysuser结构体模板为su变量分配空间 
    FILE *fp;
    //打开user.txt，用于准备往它末尾加入新的行  
    fp = fopen("user.txt", "a"); /*a参数表示：以附加的方式打开只写文件。
								若文件不存在，则会建立该文件。
								若文件存在，写入的数据会被加到文件尾，（就是不会覆盖，文件原先的内容会被保留）*/ 
	//用于检测user.txt不存在时创建user.txt文件 
    if (fp == NULL) fopen("user.txt", "w"); /*w参数表示：打开只写文件。
											若文件存在则长度清为0，即文件内所有内容消失	。
											若不存在则创建该文件。*/ 
create_loop:
    system("CLS");	//清屏（清除上一个回车的内容） 
    
    //打印注册的提示信息 
    printf("\n");
    printf("===================================注册新用户===================================");
    printf("\n\n");
    printf("  【用户账号】为11位手机号，请确保您的将要用于注册的手机号未曾用于注册本系统\n\n");
    printf("           在校学生不用注册，可直接使用11位学号+初始密码123直接登录        \n\n");
    printf("    【密码】的长度应小于等于8位，仅允许使用英文字符或数字，不允许空格\n\n");
    printf("                p.s.为保护您的隐私，输入的密码不会显示在屏幕上\n\n");
    printf("================================================================================");
    printf("\n\n");
    
    //提示输入并接收su.usernum 
    printf("                请输入新的手机号，按回车结束：");
    scanf("%s", su.usernum);
    printf("\n");
    
    //判断输入的用户账号是否被注册过
    if (check_usernum(&su)==0)
    {
        //提示输入并接收用户首次输入的密码数组su.password[i] 
        printf("                请输入新的密码，按回车结束：");
        //scanf("%s",su.password);  //scanf()里的参数必须是指针 
                                    //1、已经是指针的不加 &，比如 char name[20]中的name
                                    //2、不是指针的要加 &，比如 float math中的math 
        //printf("\n");
        int i=0;
        while(0<i<8 && (su.password[i]=getch())!='\r')//当用户输完密码并确认（按下了Enter键） 
        {   
            //用户在输入密码的过程中有删除行为（按下了backspace退格键） 
            if(su.password[i]=='\b')//su.password[i]数组长度减少i位 
            {
                if(i <= 0);
                else i--;
            }
            //正常输入密码 
            else
            {
                i++;
                //输入的密码超出了8位，提示超出8位，并且自动删除超出8位后的输入，只取前八位
                if (i > 8)
                {
                    printf("\n           注意：已经达到8位字符上限，只有前8位键入的字符有效！\n");
                    i = i - 1;
                }
            }
        }
        su.password[i]='\0';//插入字符串结尾符至su.password[i]数组 
        
        
        //提示并接收用户再次输入的密码 check_password[8] 
        char check_password[8];
        printf("\n\n                请再次输入密码，按回车结束："); 
        //scanf("%s",check_password); 
        //printf("\n"); 
        i = 0;
        for(i = 0; i < 8; i++)
            check_password[i] = '\0';
            
        i = 0;  
        while(0 < i < 8&&(check_password[i]=getch())!='\r')
        {
            if(check_password[i]=='\b')
            {
                if(i <= 0);
                else i--;
            }
            else
            {
                i++;
                if (i > 8)
                {
                    printf("\n           注意：已经达到8位字符上限，只有前8位键入的字符有效哦\n");
                    i = i - 1;
                }
            }
        }
        check_password[i]='\0';
        
        //比较su.password[i]和check_password[8] 
        int check = 1;
        if (strcmp(check_password, su.password) == 0);
        else
        {
            printf("\n\n                  两次密码输入不一致！请重新注册...\n");
            Sleep(2000);
            goto create_loop;
        }
        
        //格式化输出su变量的值(su.usernum和su.password)到user.txt文件    (往user.txt最后一行写入新注册用户) 
        fprintf(fp, "%s %s", su.usernum, su.password);
        fprintf(fp,"\n");
        fclose(fp);
        
        //打印注册成功的提示信息 
        printf("\n\n\n\n");
        printf("注册成功!请记住您的用户名：%s", su.usernum);
        printf("，还有请牢记您的密码！\n\n");
        
        //按下任意键清屏并且返回首页主菜单 
        printf("\n\n\n\n按任意键返回主菜单，选登录即可");
        char ch;
        ch = getch();
        system("cls");
        return 1;
    }
    else 
	{
		printf("                这个手机号被注册过啦，请换个手机号来注册哦\n");
  	    printf("\n                按任意键返回");
   	    char ch;
    	ch = getch();  //这个记得写。。不然不getch()的话会把上面的那两句printf()吃掉的。。神不知鬼不觉莫名其妙就goto去了不知道哪个地方。。 
   		goto create_loop;
	}    
}


//定义checkUserValid()函数 (登录时调用,检测用户合法性） 
int checkUserValid(struct sysuser *user)
{

    //打开存储用户账号密码的文件user.txt 
    FILE *user_file_p;
    user_file_p = fopen("user.txt","r");
    
    if (user_file_p == NULL) 
	user_file_p = fopen("user.txt","w");

    char usr[30], pwd[10];  //usr[30]是用户即时输入的，user->usernum和user->password都由本函数的形参user将值传入 
    char usr_f[30];  //usr_f[30]是从user.txt里读取的（用户账号+空格+密码+\n） 

    int check = 0;
	
	//将(用户名、空格、密码、\n)连接成新字符串usr，用于在user.txt文件中检查匹配
    strcpy(usr, user -> usernum);
    strcat(usr, " ");
    strcpy(pwd, user -> password);
    strcat(usr, pwd);
    strcat(usr, "\n");

    //从文件中读入用户信息数据，遍历判断用户即时输入的和user.txt里的（用户账号+空格+密码+\n）是否匹配 
    while(feof(user_file_p) == 0)
    {
        fgets(usr_f, 30, user_file_p); //从流中读一行或指定个字符，原型是char *fgets(char *s, int n, FILE *stream);
									  //从流中读取最多n-1个字符，参数s是来接收字符串的，
									  //如果成功则返回s的指针，否则返回NULL
									  //就是从user_file_p这个文件输入流中读取最多30个字符，存到user_f中
									  //fgets()是按行为单位读取的，所以参数n要设置得合理才行 
        if(strcmp(usr, usr_f) == 0)
        {
            check = 1;
            break;
        }
    }

    //关闭文件
    if(fclose(user_file_p))
    {
        printf("Can not close the file!\n");
        exit(0);
    }

    return check;
}

//登录 
int login()
{
    struct sysuser su;
    char ch;
    int check = 0;
    
    //清屏 
    system("CLS");
    
    //打印主标题 
    print_main_title();
    printf("\n\n");
    
	//提示输入并接收su.usernum 
    printf("                  请输入11位学号或手机号，按回车键结束：");
    scanf("%s", su.usernum);
    printf("\n\n");
    
    //提示输入并接收su.password 
    printf("                       请输入小于等于8位的密码\n\n");   
	printf("         该密码输入时不会显示在屏幕上，输完时请按回车键结束哦\n\n");
	printf("                               请输入吧："); //scanf("%s", su.password);
    int i=0;
    while(0 < i < 8&&(su.password[i]=getch())!='\r')
    {
        if(su.password[i]=='\b')
        {
            if(i <= 0);
            else i--;
        }
        else
        {
            i++;
            if (i > 8)
            {
                printf("\n             注意：已经达到8位字符上限，本次键入的字符将无效！\n\n");
                i = i - 1;
            }
        }
    }
    su.password[i]='\0';
    
    
	//检测用户合法性 
    if(checkUserValid(&su) == 1)
    {
        strcpy(user_number, su.usernum);
        check = 1;
        printf("\n\n\n");
        printf("                              登录成功！\n");
        printf("\n");
        printf("按任意键进入主菜单...");
        ch = getch();
        system("cls");
    }

    else
    {
        printf("\n\n\n");
        printf("            该用户不存在或密码错误，请核对后重新登录，或注册个新账号！\n\n");
        printf("                      如需帮助，请与管理员联系!\n\n");
        printf("\n\n\n\n按任意键返回主菜单...");
        ch = getch();
        system("cls");
    }

    return check;
}

/*------------------------------------------------对library.txt的操作的相关函数开始------------------------------------------------*/ 

//检查library.txt是否为空（从library.txt中读取数据至链表时调用） 
int check_void_file()
{
    FILE *fp;  //定义fp，一个指向文件类型的文件指针（FILE *）
    fp = fopen("library.txt", "r+");  //告诉fp要指向的位置是library.txt的开头（r+表示允许读写，读取和写入的光标都在文件开头） 
    char ch;
    ch = fgetc(fp); //读取文件的第一个字符 
    fclose(fp); //关闭文件 
    if (ch == EOF) return 0; //如果文件第一个字符为末尾字符（即文件为空），返回0   //EOF是个宏，用于标识已经读到文件末尾） 
    return 1; //文件非空，返回1 
}

//从library.txt中读取数据至链表
struct book *make_linklist()   //这货是条指针函数，本质就是段函数（在汇编程序里面函数就是code segment哈哈和 data segment并列的存在）
								//所以函数和数组都是差不多的东西，声明函数或数组之后默认会分配给他们一个指向他们首地址的指针
								//而且这个指针的名字跟函数名/数组名一样 
								//make_linklist()的返回值是（struct book *）    //（struct book *)就是指向类型为book的结构体的指针 
{
    struct book *head;  //指针head现在可以指向任何类型为book的结构体 
    head = (struct book *)malloc(sizeof(struct book));  //分配book结构体大小的内存，
														//并将这块内存按照book结构体的大小划分单元， 
														//然后将这块内存的首地址赋值给指针变量head，即head为头指针 
    head->next = NULL; //将头节点的指针域赋值为null，即初始化一个带头结点的单链表 
    	 
    FILE *fp;  //定义fp，一个指向文件类型的文件指针（FILE *） 
    fp = fopen("library.txt", "r+");  //告诉fp要指向的文件是library.txt     （r+表示允许读写文件） 
     
    if (fp == NULL)  //如果library.txt文件不存在 
        fp = fopen("library.txt", "w+");  // w+ ，如果文件已经存在，将建立一个新文件覆盖原文件（很缺德啊……），并且支持读取。
        							  	 //此处用于创建library.txt文件 
        							  			  	
    if (check_void_file() == 0) //检测到library.txt为空 
    {
        printf("图书数据库为空！");
        return head;  //返回创建链表的头指针，即返回整个链表，这样就可以被其他地方所用了 
    }
    
    struct book *p;
    p = head;  //给p赋值为头指针，即让p指向头节点 
    char ch;
    while(!feof(fp))  //当文件指针fp没有到达末尾，循环执行从library.txt读取数据到链表 
					  //（feof()用于检测文件是否结束,如果没有结束,返回值是0,结束了是1） 
    {
        struct book *new_book_init;   //指针new_book_init现在可以指向类型为book的任何结构体 
        new_book_init = (struct book *)malloc(sizeof(struct book));  //分配book结构体大小的内存，并将这块内存按照book结构体的大小划分单元， 
																	//然后将这块内存的首地址赋值给指针变量new_book_init，
																	//即new_book_init为头指针 
        fscanf(fp, "%d", &new_book_init->number);  //格式化输入new_book_init指针指向的结构体的number成员 
        ch = fgetc(fp); 
        fscanf(fp, "%[^\n]%*c", new_book_init->name); 
        fscanf(fp, "%[^\n]%*c", new_book_init->category); 
        fscanf(fp, "%d", &new_book_init->lent); 
        ch = fgetc(fp); 
        fscanf(fp, "%d", &new_book_init->year); 
        ch = fgetc(fp); 
        fscanf(fp, "%d", &new_book_init->month);
        ch = fgetc(fp);
        fscanf(fp, "%d", &new_book_init->day);
        ch = fgetc(fp);
        new_book_init->next = NULL;  //初始化空链表 ?

        p->next = new_book_init; 
        p = p->next;
    }
    fclose(fp);
    return head;
}




//增加书本，单链表的插入结点 
int input_new_book(struct book *head)
{
    FILE *fp;
    struct book newbook;
    struct book *p; 
    p = head; 
    if (p -> next == NULL)
    {
        printf("空链表");
    }
input_loop:
    fp = fopen("library.txt", "a+");
    system("ClS");
    printf("\n");
    printf("===================================录入新图书===================================");
    printf("\n\n    请注意：编号要求为30位以内的纯数字\n\n");
    printf("            名字要求为30个英文字符以内，可包括空格，一个汉字按两个字符计算\n\n");
    printf("            名字要求为10个英文字符以内，可包括空格，一个汉字按两个字符计算\n");
    printf("\n");
    printf("================================================================================");
    printf("\n");
input_number_loop:
    printf("                        请输入新书的编号：");
    scanf("%d",&newbook.number);
    while (p->next != NULL)
    {
        if (newbook.number == p->number)  //此处的p->number为p指向的结构体的成员变量 
        {
            printf("\n                        该书已存在！请核对后重新输入\n                        ");
            system("PAUSE");
            printf("\n");
            goto input_number_loop;
        }
        p = p->next;
    }
    p = head;  //让p指向头结点 
    printf("\n");
input_name_loop:
    printf("                        请输入新书的名字：");
    fflush(stdin);  //fflush()用于强迫将缓冲区内的数据写回stdin中 
    scanf("%[^'\n']",newbook.name);
    while (p->next != NULL) //循环遍历整个链表 
    {
        if (strcmp(p->name, newbook.name) == 0)
        {
            printf("\n                        该书已存在！请核对后重新输入\n                        ");
            system("PAUSE");
            printf("\n");
            goto input_name_loop;
        }
        p = p->next;
    }
    p = head;
    fflush(stdin);
    printf("\n");
input_category_loop:
    printf("                        请输入新书的分类：");
    scanf("%[^'\n']",newbook.category);
    fflush(stdin);
    p = head;

    newbook.lent = 0;
    newbook.year = 0;
    newbook.month = 0;
    newbook.day = 0;
    newbook.next = NULL;
    fprintf(fp,"%d ",newbook.number);
    fprintf(fp,"%s\n",newbook.name);
    fprintf(fp,"%s\n",newbook.category);
    fprintf(fp,"%d %d %d %d\n",newbook.lent,newbook.year,newbook.month,newbook.day);
    fclose(fp);
    printf("\n                        录入成功！\n\n");
    printf("                        您录入的书本资料如下：\n");
    printf("                        编号：%d\n", newbook.number);
    printf("                        书名：%s\n", newbook.name);
    printf("                        分类：%s\n", newbook.category);
    printf("\n");
    printf("                        是否继续录入？ y：是  n：否 ");
    fflush(stdin);
    char ch;
    scanf("%c", &ch);
    if (ch == 'y')
        goto input_loop;
    else
    {
        printf("\n\n即将在3秒后返回主菜单...");
        Sleep(3000);
        return 1;
    }
}

//删除书本。单链表的删除第i个结点 
struct book *delete_book(struct book *head, struct book *p)
{
    system("CLS");
    print_booklist_title();
    print_booknode(p);
     
    printf("                           警告！该操作不可恢复！\n\n");
    printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
    printf("                            是否继续？\n");
    printf("                            y：是  任意键：否\n");
    printf("                            请输入对应选项，按回车继续：");
    char ch;
    scanf("%c", &ch);
    
    if (ch == 'y')
    {
        struct book *q; 
        q = head;
        while(q->next != p)
        {
            q = q->next;
        }
        q->next = p->next;
        free(p);
        printf("删除成功！\n");
        system("pause");
        return head;
    }
    else
        return head;

}

//修改书本 
struct book *modify_book(struct book *head, struct book *t)
{
    int i;
    char name_temp[30];
    for(i = 0; i < 30; i++)
        name_temp[i] = '\0';
    i = 0;

    char cate_temp[10];
    for(i = 0; i < 10; i++)
        cate_temp[i] = '\0';
    i = 0;

    int num_temp = 0;
    fflush(stdin);
    system("CLS");
    printf("\n==================================修改图书======================================");
    printf("\n\n                           警告！该操作不可恢复！\n\n");
    printf("\n============================这是你要修改的图书==================================");
    print_booklist_title();
	print_booknode(t);
	printf("-------------------------------------------------------------------------------");
	printf("\n");
	printf("\n-----------------------------请选择你要进行的操作-----------------------------"); 
	printf("\n\n"); 
    printf("                      1、修改名称\n\n");
    printf("                      2、修改编号\n\n");
    printf("                      3、修改分类\n\n");
    //printf("4、显示所有书本\n");
    printf("                      4、我决定什么也不改我想返回主菜单\n\n");
    printf("请输入选项前的编号，按回车进入：");
    scanf("%d", &i);
    switch(i)
    {
    case 1:
        printf("名称需在30个字符以内，一个汉字为两个字符\n");
        printf("请输入新的名称：");
        fflush(stdin);
        gets(name_temp);
        strcpy(t->name, name_temp); 
        printf("修改成功！");
        system("PAUSE");
        return head;
    case 2:
        printf("编号需全为数字，且在20位以内\n");
        printf("请输入新的编号：");
        fflush(stdin);
        scanf("%d", t->number);
        printf("修改成功！");
        system("PAUSE");
        return head;
    case 3:
        printf("分类需在10个字符以内，一个汉字为两个字符\n");
        printf("请输入新的分类：");
        fflush(stdin);
        gets(cate_temp);
        strcpy(t->category, cate_temp);
        printf("修改成功！");
        system("PAUSE");
        return head;
    case 4:
        return head;
    default:
        return head;
    }
}


//覆盖链表到library.txt，将清空原library.txt的数据
void override_to_file(struct book *head)
{
    FILE *fp; 
    fp = fopen("library.txt", "w");
    struct book *p;
    p = head->next;  //让p指向第二个结点 
    while(p->next != NULL)
    {
        fprintf(fp, "%d ", p->number);
        fprintf(fp, "%s\n", p->name);
        fprintf(fp, "%s\n", p->category);
        fprintf(fp, "%d ", p->lent);
        fprintf(fp, "%d ", p->year);
        fprintf(fp, "%d ", p->month);
        fprintf(fp, "%d", p->day);
        fprintf(fp, "\n");
        p = p->next;   //让p指向下一个节点 
    }
    fclose(fp);
}



//查找书本（按编号） 
struct book *search_by_number(struct book *head, int number)
{
    struct book *p;
    p = head->next; //让p指向头结点 
    if (p == NULL)
        return NULL;
    while(p -> next != NULL) //循环遍历整个链表 
    {
        if (p -> number == number)
            return p;
        p = p->next;
    }
    //return NULL;
}

//查找书本（按书名） 
struct book *search_by_name(struct book *head, char *name)
{
    struct book *p;
    p = head->next; //让p指向头结点 
    if (p == NULL)
        return NULL;
    while(p != NULL)
    {
        if (strcmp(name, p->name) == 0)
            return p;
        p = p->next;
    }
    //return NULL;
}

//查找书本（按分类） 
int search_by_category(struct book *head, char *category)
{
    struct book *p;
    p = head->next; //让p指向头结点 
    int flag = 0;
    while(p -> next != NULL)
    {
        if (strcmp(category, p->category) == 0) //比较两个字符串是否相等 
        {
            print_booknode(p);
            flag = 1;
        }
        p = p->next;
    }
    if(flag == 0)
    {
        printf("没有找到该分类的书！\n");
        printf("按任意键返回主菜单...\n");
        getch();
        return 0;
    }
    printf("按任意键返回主菜单...\n");
    getch();
    return 0;

}

//查找书本（按借出时间） 
struct book *search_by_time(struct book *head, int year, int month, int day)
{
    int flag = 0;
    struct book *p;
    p = head->next;  //让p指向头结点 
    if (p == NULL)
        flag = 0;
    while(p -> next != NULL) //循环遍历 
    {
        if ((p->year == year) && (p->month == month) && (p->day == day))
            {
                print_booknode(p);
                flag = 1;
            }
        p = p->next;
    }
    if (flag == 0)
        printf("没有找到书本！\n");
    system("PAUSE");
}

//输出该书的信息（编号+书名+分类+在库/借出时间） 
void print_booklist_title()
{
    printf("您查找的书的资料是：\n");
    printf("--------------------------------------------------------------------------------");
    printf("编号                书名                          分类        借出时间       \n");
    printf("--------------------------------------------------------------------------------");
}
void print_booknode(struct book *t)
{

    if(t != NULL)
    {
        printf("%-20d%-30s%-10s",t->number, t->name, t->category);
        if(t->lent == 1)  
        {
            printf("  %d年%d月%d日\n",t->year,t->month,t->day);
        }
        else printf("  在库\n");
    }
    else
    {
        printf("没有找到该书！\n");
        printf("按任意键返回主菜单...\n");
        getch();
    }
}




//输出所有书
int print_all_book(struct book *head)          
{
	//清屏 
    system("CLS");
    //打印小标题 
    printf("\n");
    printf("================================显示所有书本====================================");
    printf("\n");
    // 
    char ch;
    // 
    struct book *p;
    p = head->next;
    // 
    if (p == NULL)
    {
        printf("\n数据库中没有书本！\n\n");
        printf("                  按任意键返回...");
        ch = getch();
        return 0;
    }
    // 
    print_booklist_title();
    do
    {
        print_booknode(p);
        p = p->next;
    }
    while(p->next != NULL);
    printf("\n按任意键返回上级菜单哦...");
    ch = getch();
    return 1;
}

/*---------------------------------------------------对library.txt的操作相关函数结束---------------------------------------------*/ 



/*---------------------------------------------------对user.txt的操作相关函数开始---------------------------------------------*/
//检查user.txt是否为空（从user.txt中读取数据至链表时调用） 
int check_sysuser_void_file()
{
    FILE *fp;  //定义fp，一个指向文件类型的文件指针（FILE *）
    fp = fopen("user.txt", "r+");  //告诉fp要指向的位置是library.txt的开头（r+表示允许读写，读取和写入的光标都在文件开头） 
    char ch;
    ch = fgetc(fp); //读取文件的第一个字符 
    fclose(fp); //关闭文件 
    if (ch == EOF) return 0; //如果文件第一个字符为末尾字符（即文件为空），返回0   //EOF是个宏，用于标识已经读到文件末尾） 
    return 1; //文件非空，返回1 
}
//从user.txt中读取数据至链表
struct sysuser *make_sysuser_linklist()   //这货是条指针函数，本质就是段函数
										 //（在汇编程序里面函数就是code segment，和data segment并列的存在）
									   	//它的返回值类型是(struct sysuser *),就是一个指向类型为sysuser结构体的指针 
{
    struct sysuser *sysuser_head;  //指针head现在可以指向任何类型为的结构体 
    sysuser_head = (struct sysuser *)malloc(sizeof(struct sysuser));  //分配sysuser结构体大小的内存，
															//并将这块内存按照sysuser结构体的大小划分单元， 
															//然后将这块内存的首地址赋值给指针变量sysuser_head，即sysuser_head为头指针 
    sysuser_head->next = NULL; //将头节点的指针域赋值为null，即初始化一个带头结点的单链表 
    	 
    FILE *fp;  //定义fp，一个指向文件类型的文件指针（FILE *） 
    fp = fopen("user.txt", "r+");  //告诉fp要指向的文件是user.txt     （r+表示允许读写文件） 
     
    if (fp == NULL)  //如果user.txt文件不存在 
        fp = fopen("user.txt", "w+");  // w+ ，如果文件已经存在，将建立一个新文件覆盖原文件（很缺德啊……），并且支持读取。
        							  	 //此处用于创建user.txt文件 
        							  			  	
    if (check_sysuser_void_file() == 0) //检测到user.txt为空 
    {
        printf("用户信息据库为空！");
        return sysuser_head;  //返回创建链表的头指针，即返回整个链表，这样就可以被其他地方所用了 
    }
    
    struct sysuser *p;
    p = sysuser_head;  //给p赋值为头指针，即让p指向头节点 
    char ch;
    while(!feof(fp))  //当文件指针fp没有到达末尾，循环执行从user.txt读取数据到链表 
					  //（feof()用于检测文件是否结束,如果没有结束,返回值是0,结束了是1） 
    {
        struct sysuser *new_sysuser_init;   //指针new_sysuser_init现在可以指向类型为book的任何结构体 
        new_sysuser_init = (struct sysuser *)malloc(sizeof(struct sysuser));  //分配sysuser结构体大小的内存，并将这块内存按照sysuser结构体的大小划分单元， 
																	//然后将这块内存的首地址赋值给指针变量new_sysuser_init，
                                                                    //(struct sysuser *)是强制类型转换(type)，强制转换为指向sysuser类型的结构体的指针
																	//即new_sysuser_init为头指针 
        fscanf(fp, "%s", &new_sysuser_init->usernum);  //格式化输入new_sysuser_init指针指向的结构体的number成员 
        ch = fgetc(fp); //读取一个空格，并且与此同时fp指针自动下移到下一个还未操作过但即将要操作的字符
        fscanf(fp, "%[^\n]%*c", new_sysuser_init->password); 
        //%[]表示要读取的指定字符集的文本。eg: %[0-9]表示只读取数字，遇到非数字字符就停止。%[\n]表示只读取\n（它代表换行）这个字符
        //%[^]表示读取其补集。eg：%[^\n]表示读取换行符之外的所有字符
        //%*表示忽略读取到的字符，就是说读取到了指定的字符，但是不保存到变量中。
        //所以%[^\n]%*c表示读取到了一整行（包括了\n）但是只保存这一整行里面除了\n之外的所有字符【总而言之%*c是用来吃回车符的。。】
        new_sysuser_init->next = NULL;  //初始化空链表 

        p->next = new_sysuser_init; 
        p = p->next;
    }
    fclose(fp);
    return sysuser_head;
}


//查找用户（按用户账号） 
struct book *search_by_usernum(struct sysuser *sysuser_head, char *del_sysuser_num)
{
    struct sysuser *p;
    p = sysuser_head->next; //让p指向头结点 
    if (p == NULL)
        return NULL;
    while(p != NULL)
    {
        if (strcmp(del_sysuser_num, p->usernum) == 0)
            return p;
        p = p->next;
    }
    //return NULL;
}
//删除用户
struct sysuser *delete_sysuser(struct sysuser *sysuser_head, struct sysuser *p) 
{
	struct sysuser *q; 
    q = sysuser_head;
    while(q->next != p)
    {
        q = q->next;
    }
    q->next = p->next;
    free(p);
    printf("删除成功！\n");
    system("pause");
    return sysuser_head;
}
//覆盖链表到user.txt，将清空原user.txt的数据
void override_to_sysuser_file(struct sysuser *sysuser_head)
{
    FILE *fp; 
    fp = fopen("user.txt", "w");
    struct sysuser *p;
    p = sysuser_head->next;  //让p指向第二个结点 
    while(p->next != NULL)
    {
        fprintf(fp, "%s ", p->usernum);
        fprintf(fp, "%s\n", p->password);
        p = p->next;   //让p指向下一个节点 
    }
    fclose(fp);
}


//输出该用户的信息（用户名+密码） 
void print_sysuserlist_title()
{
    printf("您查找的用户的资料是：\n");
    printf("--------------------------------------------------------------------------------");
    printf("   用户账号                                                      密码           \n");
    printf("--------------------------------------------------------------------------------");
}
void print_sysusernode(struct sysuser *m)
{
    if(m != NULL)
    {
        printf("%-64s%-10s\n",m->usernum, m->password);
    }
    else
    {
        printf("没有找到该用户！\n");
        printf("按任意键返回主菜单...\n");
        getch();
    }
}

//输出所有用户 
int print_all_sysuser(struct sysuser *sysuser_head)          
{
	//清屏 
    system("CLS");
    //打印小标题 
    printf("\n");
    printf("================================显示所有用户====================================");
    printf("\n");
    // 
    char ch;
    // 
    struct sysuser *p;
    p = sysuser_head->next;
    // 
    if (p == NULL)
    {
        printf("\n数据库中没有任何用户信息！\n\n");
        printf("                  按任意键返回...");
        ch = getch();
        return 0;
    }
    // 
    print_sysuserlist_title();
    do
    {
        print_sysusernode(p);
        p = p->next;
    }
    while(p->next != NULL);
    printf("\n按任意键返回上级菜单哦...");
    ch = getch();
    return 1;
}
/*---------------------------------------------------对user.txt的操作相关函数结束---------------------------------------------*/


//主函数
int main()
{
	//定义login_temp编号（编号具体对应功能由以下一堆printf（""）输出到屏幕）
    char login_temp;    
loop:
	//打印欢迎标题+菜单 
    print_main_title();
    printf("\n\n");
    printf("                               1、注册\n\n");
    printf("                               2、登录\n\n");
    printf("                               3、随便逛逛\n\n");
    printf("                               4、退出\n\n");
    printf("\n\n");
    printf("                      请输入选项前的编号，按回车键结束：");
    //接收用户输入的login_temp编号
    scanf("%c",&login_temp); 
    
    //234当中要用到的变量 
    int login_check_temp=1; //2.登录   
    char visitor_select; //3.随便逛逛
    char login_exit_temp; //4.退出  
        
    switch(login_temp)
    {
    //1.注册 
    case '1':
        create_user();
        goto loop;
    //2.登录 
    case '2':
    	//调用login()函数，顺便求login_check_temp这个临时变量的值 
        login_check_temp = login();
        //登录不成功则返回主菜单 
        if(login_check_temp == 0)goto loop;   
		//登录成功则修改标志以改变欢迎标题显示 
	    check_login_success = 1;
	    
	    // 
	    char menu_select; //登录后见到的菜单编号
		int reader_select;//1,读者
		
		int user_admin_select;//2,系统管理员
		int del_sysuser_select;//2.1删除用户 
			    
		int book_admin_select; //3,图书管理员
		int search_select; //查找
		int borrow_select; //3.1,借书 
	    int back_select; //3.2,还书 
		int modify_select;//3.4,修改图书 
		int admin_delete_select;//3.5,删除图书 
		
		struct sysuser *sysuser_head; 
		struct book *head;
	    char ch;
	    char str_tmp[100];
	    while(1)
	    {	
	    	//从library.txt中读取数据到链表
	        head = make_linklist();   
	main_loop:
			//清屏 
	        system("CLS");
	        //打印欢迎标题+菜单 
	        print_main_title();
	        printf("\n");

	        printf("                                1、我是【读者】\n\n");
	        printf("                                2、我是【系统管理员】\n\n");
	        printf("                                3、我是【图书管理员】\n\n");
	        printf("                                4、退出\n");
	        printf("\n");
	        printf("                       请输入要使用功能的编号，按回车进入：");
	        
			//获取所选meau_select的值 
	        scanf("%c", &menu_select);
	        
	        //声明一个指向类型为sysuser的结构体的指针m 
	        struct sysuser *m;	        
			//声明一个指向类型为book的结构体的指针t	 
	        struct book *t;
	        
	        //根据编号查找，编号=temp 
	        int temp = 0;
	        //根据书名查找，书名=name_temp[30]
	        int i;
	        char name_temp[30];
	        for(i = 0; i < 30; i++) name_temp[i] = 0;
	        //根据分类查找 ,分类=category_temp[10] 
	        char category_temp[10];
	        for(i = 0; i < 10; i++) category_temp[i] = 0;
			//根据借出时间查找， 
	        time_t timep; //time_t实际上就是长整型long int;他用来保存从1970年1月1日0时0分0秒到现在时刻的秒数！用time()这个函数获取！
	        struct tm *time_p;
	        time(&timep);
	        time_p = gmtime(&timep);	
	        int year = 0;
	        int month = 0;
	        int day = 0;
	
	        switch(menu_select)
	        {
	        //读者 
	        case '1':
	            break;
	        //系统管理员 
	        case '2':
user_admin_loop:	        	
	        	system("CLS");
	            print_main_title();
	            printf("\n\n");
	            printf("                        1、删除用户\n\n");
	            printf("                        2、列出所有用户\n\n");
	            printf("                        3、返回上一级\n");
	            printf("\n\n");
	            printf("                        请输入对应功能的编号，按回车进入：");
	            scanf("%d", &user_admin_select);
	            struct sysuser *m;
				while(1){
					//从user.txt中读取数据到链表 
	        		struct sysuser *sysuser_head;
					sysuser_head=make_sysuser_linklist(); 
	          	    switch(user_admin_select){
	          	  		//删除用户 
						case 1:
							system("CLS");
							printf("\n\n");
							printf("=====================================删除用户===================================\n\n"); 
							printf("                                     请谨慎操作！                               \n\n"); 
							printf("================================================================================\n\n");
							printf("                        系统需定位您要删除的用户：\n\n");
			                printf("                        1、输入用户的11位的用户账号\n\n");
			                printf("                        2、返回\n\n");
			                printf("                请输入以上对应选项的编号，按回车继续：");
			                scanf("%d", &del_sysuser_select);
			                printf("\n\n");														
							switch(del_sysuser_select)
							{
								case 1:
									printf("请输入你要删除的用户的账号（11位）：");
									char temp[12];
									scanf("%s",&temp);
									printf("\n\n"); 
									t = search_by_usernum(sysuser_head, temp);
				                    if (t == NULL)
				                    {
				                        printf("没有找到该用户！请核对后再重新输入\n");
				                        printf("按任意键返回菜单...\n");
				                        getch();
				                        break;
				                    }
				                    else
				                    {
				                        sysuser_head = delete_sysuser(sysuser_head,t);  //删除用户 
				                        override_to_sysuser_file(sysuser_head);  //覆盖写入user.txt 
				                        break;
				                    }
				                    break;
								case 2:
									goto user_admin_loop;	
							}
							break; 
						//列出所有用户 
						case 2:
							print_all_sysuser(sysuser_head); 
							goto user_admin_loop;
					 		break;
					} 
				} 
		       break;
	        //图书管理员
	        case '3': 
book_admin_loop:
				system("CLS");
	            print_main_title();
	            printf("\n\n");
	            printf("                        1、借书\n\n");
	            printf("                        2、还书\n\n");
	            printf("                        3、录入新图书到数据库\n\n");
	            printf("                        4、修改已有图书的数据\n\n");
	            printf("                        5、从数据库中删除图书\n\n");
	            printf("                        6、返回上一级\n");
	            printf("\n\n");
	            printf("                        请输入对应功能的编号，按回车进入：");
	            scanf("%d", &book_admin_select);
	            switch(book_admin_select)
	            {
		        //借书
		        case 1:
	            	//清屏+打印标题和菜单+接收输入 
	borrow_loop:    system("CLS");
		            printf("\n");
		            printf("======================================借书======================================");
		            printf("\n");
		            printf("                        系统需要定位您要借的书：\n\n");
		            printf("                        1、请在此输入书的编号\n\n");
		            printf("                        2、返回上一级\n\n");
		            printf("                        请输入对应选项的编号，按回车进入：");
		            scanf("%d", &borrow_select);
		            //开关箱子啦 
		            switch(borrow_select)
		            {         
		            case 1:
		            	//提示 
		                printf("\n\n");
		                printf("                    请输入书的编号，按回车结束：");
		                scanf("%d", &temp);
		                //按编号查找定位要借的书本
		                t = search_by_number(head, temp);  
		                //time_t timep;
		
		                if (t != NULL)
		                {
		                    system("CLS");
		                    printf("\n");
		                    printf("======================================借书======================================");
		                    printf("\n\n");
		
		                    print_booklist_title();
		                    print_booknode(t);
		                    if(t->lent == 0)
		                    {
		                        printf("\n\n");
		                        printf("                        您确定要借这本书吗？1：是 2：否");
		                        scanf("%d", &temp);
		                        if(temp == 1)
		                        {
		                            t->lent = 1;
		                            t->year = 1900 + time_p->tm_year;
		                            t->month = time_p->tm_mon;
		                            t->day = time_p->tm_mday;
		
		                            printf("\n\n");
		                            printf("                        借出成功！");
		                            override_to_file(head);
		                            system("PAUSE");
		                            goto borrow_loop;
		                            break;
		                        }
		                    }
		                    //else break;
		                    printf("\n\n\n");
		                    printf("                    错误：该书已经借出！请核对后再重新输入\n\n");
		                    printf("                    如需帮助，请联系工作人员\n\n                    ");
		                    system("PAUSE");
		                    goto borrow_loop;
		                    break;
		                }
		                else
		                {
		                    printf("\n\n\n");
		                    printf("                    错误：没有找到该书！请核对后再重新输入\n\n");
		                    printf("                    如需帮助，请联系工作人员\n\n                    ");
		                    system("PAUSE");
		                    goto borrow_loop;
		                    break;
		                }
		             case 2:
		             	goto book_admin_loop;
		             default:
					 	goto borrow_loop;
		                break;
				   }
				   break;
				//还书 
	            case 2:
		        back_loop:
		            system("CLS");
		            printf("\n");
		            printf("======================================还书======================================");
		            printf("\n");
		            printf("                        系统需要定位您要还的书：\n\n");
		            printf("                        1、请在此输入书的编号\n\n");
		            printf("                        2、返回上一级\n\n");
		            printf("                        请输入对应功能前的编号，按回车继续：");
		
		            scanf("%d", &back_select);
		
		            switch(back_select)
		            {
		            case 1://编号
		                printf("\n\n");
		                printf("                        请输入编号，按回车结束：");
		                scanf("%d", &temp);
		                t = search_by_number(head, temp);  //按图书编号定位要还的图书 
		
		                if (t != NULL)
		                {
		                    system("CLS");
		                    printf("\n");
		                    printf("======================================还书======================================");
		                    printf("\n\n");
		
		                    print_booklist_title();
		                    print_booknode(t);
		                    if(t->lent == 1)
		                    {
		                        printf("\n\n");
		                        printf("                        您确定要还这本书吗？1：是 2：否");
		                        scanf("%d", &temp);
		                        if(temp == 1)
		                        {
		                            t->lent = 0;
		                            t->year = 0;
		                            t->month = 0;
		                            t->day = 0;
		                            //print_booknode(t);
		                            printf("\n\n");
		                            printf("                        还书成功！");
		                            override_to_file(head);
		                            system("PAUSE");
		                            goto back_loop;
		                            break;
		                        }
		                    }
		                    //else
		                    printf("\n\n\n");
		                    printf("                    错误：该书已经在库！请核对后再重新输入\n\n");
		                    printf("                    如需帮助，请联系工作人员\n\n                    ");
		                    system("PAUSE");
		                    goto back_loop;
		                    break;
		                }
		                else
		                {
		                    printf("\n\n\n");
		                    printf("                    错误：没有找到该书！请核对后再重新输入\n\n");
		                    printf("                    如需帮助，请联系工作人员\n\n                    ");
		                    system("PAUSE");
		                    goto back_loop;
		                    break;
		                }
		                break;
		            default:
		                goto book_admin_loop;//break就是跳出到上层菜单，只有要回到本层时，才需要加个loop标识符 
		            }
		            break;
	            //录入
	            case 3:
	                input_new_book(head);
	                goto book_admin_loop;
				//修改
	            case 4:
	modify_loop:
	                system("CLS");
	                printf("\n");
	                printf("====================================修改图书====================================");
	                printf("\n");
	                printf("                           警告！该操作不可恢复！\n\n");
	                printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
	                printf("\n");
	                printf("           在这里您可以修改数据库中书的编号、名称、分类和借出时间\n\n");
	                printf("                        系统需定位您要修改数据的书：\n\n");
	                printf("                        1、在此输入书的编号\n\n");
	                printf("                        2、返回\n\n");
	                printf("                请输入对应选项的编号，按回车继续：");
	                scanf("%d", &modify_select);
	                printf("\n\n");
	                switch(modify_select)
	                {
	                case 1://输入编号 
	                    fflush(stdin);
	                    printf("                    请输入书的编号，按回车结束：");
	                    scanf("%d", &temp);
	                    printf("\n\n"); 
	                    t = search_by_number(head, temp); //按编号定位图书 
	                    if (t == NULL)
	                    {
	                        printf("没有找到该书！请核对后再重新输入\n");
	                        printf("按任意键返回主菜单...\n");
	                        getch();
	                        break;
	                    }
	                    else
	                    {
	                        head = modify_book(head, t);  //修改图书 
	                        override_to_file(head);  //覆盖写入library.txt 
	                        break;
	                    }
	                    break;
	                case 2:
	                    break;
	                default:
	                    break;
	                }
	                break;
	                //对t进行修改
	                
				//删除
	            case 5:
	                system("CLS");
	                printf("\n");
	                printf("====================================删除图书====================================");
	                printf("\n");
	                printf("                           警告！该操作不可恢复！\n\n");
	                printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
	                printf("\n\n");
	                printf("\n\n");
	                printf("                        系统需定位您要删除的书：\n\n");
	                printf("                        1、根据编号查找\n\n");
	                printf("                        2、返回\n\n");
	                printf("                        请输入对应选项的编号，按回车继续："); 
	                scanf("%d", &admin_delete_select);
	                printf("\n\n");
	                switch(admin_delete_select)
	                {
	                case 1://按编号删除
	                    system("CLS");
	                    printf("                           警告！该操作不可恢复！\n\n");
	                    printf("                        WARNING!THIS CAN'T BE UNDONE!!\n\n");
	                    printf("                       请输入要删除的书的编号，按回车结束：");
	                    scanf("%d", &temp);
	                    printf("\n\n");
	                    fflush(stdin);
	                    t = search_by_number(head, temp);
	                    if (t != NULL)
	                    {
	                        head = delete_book(head, t);
	                        override_to_file(head);
	                    }
	                    else
	                    {
	                        printf("没有找到此书！\n");
	                        system("PAUSE");
	                    }
	                    break;
	                case 2:
	                    break;
	                }
	            default:
	                break;
	            }
	            break;          
	          //返回上一级 
	          case '4': 
		           {
		            printf("\n");
		            printf("                      您确定要退出吗？\n");
		            printf("                      按任意键退出，按n返回\n");
		            login_exit_temp = getch();
		            if(login_exit_temp == 'n')
		            {
		                system("CLS");
		                goto loop;
		            }
		            else return 1;
		           }
		           break;
			 //输入了除了123456的其他鬼东西 
	         default:
	             goto main_loop;
	        }
	    }
	    break;
    //3.随便逛逛 
    case '3':                                //1.列出所有书本  //2.查找书本
visitor_loop:
		    //清屏 
	        system("CLS");
	        //打印欢迎标题+菜单 
	        print_main_title();
	        printf("\n\n");
	        printf("                        1、列出所有图书\n\n");
	        printf("                        2、查找图书\n\n");
	        printf("                        3、返回上一层\n");
	        printf("\n\n");
	        printf("                        请输入对应功能的编号，按回车进入：");
	        
	        //接收用户选择的编号 
	        fflush(stdin); //fflush()用于强迫将缓冲区内的数据写回stdin中!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			               //这里弄了老子一晚上！！！！！没了这句只有scanf()的话是输不进东西进去的。。。。。。 
	        scanf("%c",&visitor_select); 
	        
	        //2查找图书
			int visitor_search_select;  
	        //根据编号查找，编号=temp 
	        int temp = 0;
	        //根据书名查找，书名=name_temp[30]
	        int i;
	        char name_temp[30];
	        for(i = 0; i < 30; i++) name_temp[i] = 0;
	        //根据分类查找 ,分类=category_temp[10] 
	        char category_temp[10];
	        for(i = 0; i < 10; i++) category_temp[i] = 0;
			//根据借出时间查找， 
	        time_t timep; //time_t实际上就是长整型long int;他用来保存从1970年1月1日0时0分0秒到现在时刻的秒数！用time()这个函数获取！
	        struct tm *time_p;
	        time(&timep);
	        time_p = gmtime(&timep);
	
	        int year = 0;
	        int month = 0;
	        int day = 0;
	        
	        struct book *t;  
	        while(1){  	
		        //读取library.txt到链表 
		        struct book *visitor;
		        visitor = make_linklist();
		        //开关箱子啦！！！！！！！！！！！！！！！！！啊啊啊case里面的1，2，3是int型的，
				//这里visitor_select我们定的是char,所以要写'1','2','3' 
		        switch(visitor_select){
			        case '1':
						 print_all_book(visitor);
						 goto visitor_loop;
						 break; 
					case '2':
visitor_search_loop:    system("CLS");
			            print_main_title();
			            printf("\n\n");
			            printf("                        1、根据名字查找\n\n");
			            printf("                        2、根据编号查找\n\n");
			            printf("                        3、根据分类查找\n\n");
			            printf("                        4、根据借出时间查找\n\n");
			            printf("                        5、返回上级菜单\n");
			            printf("\n\n");
			            printf("                        请输入对应功能的编号，按回车进入：");
			            scanf("%d", &visitor_search_select);
			            printf("\n\n");
			            switch(visitor_search_select)
			            {
				            case 1://根据名字
				                printf("                      请输入要查找的书名,按回车结束:");
				                scanf("%s", name_temp);
				                printf("\n\n");
				                t = search_by_name(visitor, name_temp);
				                if (t != NULL)
				                {
				                	print_booklist_title();
				                    print_booknode(t);
				                    printf("                               按任意键返回主菜单...\n\n");
				                    getch();
				                    goto visitor_search_loop; 
				                }
				                else
				                {
				                    printf("没有找到该书！\n");
				                    printf("按任意键返回主菜单...\n");
				                    getch();
				                    goto visitor_search_loop;
				                }
				                break;
				            case 2://根据编号
				                printf("                       请输入要查找的书的编号,按回车结束：");
				                scanf("%d", &temp);
				                printf("\n\n");
				                struct book *t;
				                t = search_by_number(visitor, temp);
				                if (t != NULL)
				                {
				                	print_booklist_title();
				                    print_booknode(t);
				                    printf("按任意键返回主菜单...\n\n");
				                    getch();
				                    goto visitor_search_loop;
				                }
				                else
				                {
				                    printf("没有找到该书！\n");
				                    printf("按任意键返回主菜单...\n");
				                    getch();
				                    goto visitor_search_loop;
				                }
				                break;
				            case 3://根据分类
				                system("CLS");
				                printf("            请输入要查找的分类,按回车结束：");
				                scanf("%s", category_temp);
				                printf("\n\n");
				                search_by_category(head, category_temp);
				                printf("按任意键返回主菜单...\n");
				                getch();
				                goto visitor_search_loop;
				            case 4://根据借出时间
				                system("CLS");
				                printf("\n");
				                printf("================================根据借出时间查找================================");
				                printf("\n");
				                printf("\n请输入要查找的书本的借出年份,按回车结束：");
				                scanf("%d", &year);
				                printf("\n请输入月份,按回车结束：");
				                scanf("%d", &month);
				                printf("\n请输入日期,按回车结束：");
				                scanf("%d", &day);
				                printf("\n\n");
				                search_by_time(head, year, month, day);
				                goto visitor_loop;
				            case 5://返回上一级
								goto visitor_loop; 
				            default:
				                goto visitor_search_loop;
		                    }
		             case '3':
					 	goto loop;
					 default:
					 	goto visitor_loop; 
					 } 
			}
	        break; 
    //4.退出  
    case '4':
    	//打印提示信息 
        printf("\n\n");
        printf("                      您确定要退出吗？\n\n");
        printf("                      按任意键退出，按n返回\n\n");
        //接受用户退出时输入的login_exit_temp的值 
        login_exit_temp = getch();
        //输入'n'则返回，输入'任意键'则退出 
        if(login_exit_temp == 'n')
        {
            system("CLS");
            goto loop;
        }
        else exit(0);
    //输入了1234以外的鬼东西 
    default:
        system("CLS");
        goto loop;
     
  }
}








