#include <stdio.h>
struct student {
	int num;
	char * name;
};

struct student test = {
	.num = 1,
	.name = "test"
};
int main()
{
	printf("num : %d, name : %s\n",test.num, test.name);

	struct student st = {1, "ksh"};
	printf("num : %d, name : %s\n",st.num, st.name);

	struct student sa[2] = {{1, "ksh"},{2,"aaa"}};
	printf("num : %d, name : %s\n",sa[0].num, sa[0].name);
	printf("num : %d, name : %s\n",sa[1].num, sa[1].name);

	struct student * pSt;
	pSt = &st;
	printf("num : %d, name : %s\n",pSt->num, pSt->name);
}
