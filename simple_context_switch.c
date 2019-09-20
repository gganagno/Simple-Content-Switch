#include "thread.h"

struct args {
	int x;
	int y;
};

void foo(struct args *p){
	while(1)
		printf("THREAD FOO %d\n",p->x);
}

void bar(struct args *p){
	while(1);
		// printf("THREAD BAR %d %d\n",p->x, p->y);
	
}


int main(int argc, char ** argv){

	int nbytes;
	struct args u;


	Thread_init();

	
	ucontext_t ctx;
	getcontext(&ctx);

	unsigned long eip = ctx.uc_mcontext.gregs[REG_EIP];

	unsigned long esp = ctx.uc_mcontext.gregs[REG_ESP];

		printf("eip : %lx\n",eip);
		printf("esp : %lx\n",esp);
	

	u.x =1;

	u.y =2;

	nbytes=sizeof(struct args);

	  eip = ctx.uc_mcontext.gregs[REG_EIP];

	  esp = ctx.uc_mcontext.gregs[REG_ESP];

		printf("eip : %lx\n",eip);
		printf("esp : %lx\n",esp);


	// Thread_new((int(*)(void *))bar,&u,(size_t)nbytes,NULL);

	// Thread_new((int(*)(void *))foo,&u,(size_t)nbytes,NULL);
	// while(1);
	// Thread_new((int(*)(void *))foo,&u,(size_t)nbytes,NULL);
	return 0;
}	