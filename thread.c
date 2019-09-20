#include "thread.h"

void _BEGINMONITOR(void){}

void Thread_init(void){

	printf("Thread_init\n");
	MAX_THREADS = 2;
	CURR_THREAD = 0;
	int i;

	for(i=0; i < MAX_THREADS ; ++i){
		thread_array[i] = (thread_t*)malloc(sizeof(thread_t));
		thread_array[i]->tid = i;
	}
	rq = (RQ*)malloc(sizeof(RQ));
	threadlib =0;
	curr_flag =0;

	////////////////new

	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction)); //?
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = sig_handler;
	sigemptyset(&act.sa_mask);
	sigaction(SIGALRM,&act,NULL);

	alarm(1); ///?
}

void * thread_memcpy(void * dest , void * src , size_t nbytes){

	if(threadlib)return NULL;
	threadlib = 1;
	memcpy(dest,src,nbytes);
	threadlib= 0;
}



int Thread_new(int func(void *), void * args, size_t nbytes, ...){

	assert(func!=NULL && args!=NULL && nbytes>0);

	int tid=666;
	int fail =0;
	int debug =0;
	unsigned char * func_args = calloc(1,nbytes);

	if(func_args == NULL)printf("malloc failed\n");

	if(debug){
		printf("%p %p %d\n",func_args,args,nbytes);

		printf("%d\n",*(int*)args );

		printf("%d\n",*(int*)func_args );

	}

	if(thread_memcpy(func_args,args,nbytes) == NULL) return -1;

	if(debug)printf("%d\n",*(int*)func_args );

	int curr=0;
	if(curr_flag)return -1;
	curr_flag=1;
	curr = CURR_THREAD;
	CURR_THREAD++;
	curr_flag=0;

	ucontext_t ctx;
	getcontext(&ctx);

	thread_array[curr]->funcptr = func;

	thread_array[curr]->args = func_args;

	unsigned long esp = ctx.uc_mcontext.gregs[REG_ESP];

	thread_array[curr]->sp = esp;

	rq_enqueue(thread_array[curr]);


	/////// gia ta variadic arguments

	// va_list vlist;
 	// int vargs;
 	// va_start(vlist, nbytes);
 	// vargs = va_arg(vlist, int);

    ////////////////////////////

	// void * addr;

	// if(_BEGINMONITOR < addr < _ENDMONITOR){
	// 	smthg
	// 	printf("BEGIN: %p\n",(int(*)(void *))_BEGINMONITOR );
	// 	printf("END: %p\n",(int(*)(void *))_ENDMONITOR );
	// }
	

	alarm(1);

	unsigned long eip = ctx.uc_mcontext.gregs[REG_EIP];

	printf("eip : %lx\n",eip);

	printf("esp : %lx\n",esp);

	func((void*)args);

	if(!fail){
		return thread_array[CURR_THREAD-1]->tid;
	}else{
		return -1;
	}
}


void Thread_exit(int code){

 	printf("Thread_exit %d\n",code );
}


int Thread_self(void){	/*gyrnaw pisw to id mou */
	printf("Thread_self\n"); 

	int self_id = 555;
	return self_id;
}


int Thread_join(int tid){	 /* egw perimenw ton tid na teleiwsei gia na synexisw */

	printf("Thread_join %d\n",tid );
	return 0;
}


void Thread_pause(void){
	printf("Thread_pause\n"); // edw 8a ginei to content swtich?
}


/////////////////////////////////////////////////

void print_sigset_t(sigset_t *set)
{
	int i;

	i = SIGRTMAX;
	do {
		int x = 0;
		i -= 4;
		if (sigismember(set, i+1)) x |= 1;
		if (sigismember(set, i+2)) x |= 2;
		if (sigismember(set, i+3)) x |= 4;
		if (sigismember(set, i+4)) x |= 8;
		printf("%x", x);
	} while (i >= 4); 
	printf("\n");
}

void sig_handler(int signo, siginfo_t *info, void *context){

	int debug=0;
	printf(" SIZE :D%d\n",sizeof(signo) + sizeof(siginfo_t) + sizeof(ucontext_t) );
	if(debug){
		printf("siginfo_t\n");
		printf("signo: %d\n",signo); // einai to 14 pou einai (SIGALRM | SIG_INFO )

		printf("si_signo: %d\n",info->si_signo);
		printf("si_code: %d\n",info->si_code);
		printf("si_value: %d\n",info->si_value.sival_int);
		printf("si_errno: %d\n",info->si_errno);
		printf("si_pid: %d\n",info->si_pid);
		printf("si_uid: %d\n",info->si_uid);
		printf("si_addr: %p\n",info->si_addr);
		printf("si_status: %d\n",info->si_status);
		printf("si_band: %lu\n",info->si_band);
	
	}

	// if(debug){
		printf("context");
		ucontext_t ctx = *(ucontext_t*)context;
		printf("ctx.uc_link : %p \n",ctx.uc_link);
		print_sigset_t(&ctx.uc_sigmask);
		printf("ctx.ss_sp : %p \n",ctx.uc_stack.ss_sp);
		printf("ctx.ss_flags : %d \n",ctx.uc_stack.ss_flags);
		printf("ctx.ss_size : %zu \n",ctx.uc_stack.ss_size);


		unsigned long eip = ctx.uc_mcontext.gregs[REG_EIP];

		unsigned long esp = ctx.uc_mcontext.gregs[REG_ESP];
		//autos o stack pointer einai 16bytes beosta apo ths main
		printf("eip : %lx\n",eip);
		printf("esp : %lx\n",esp);

		printf("\n\n\n");

	// }

	thread_t * from = rq_dequeue();

	rq_enqueue(from);
	thread_t * to = rq_top();

	// printf("from id: %d to id: %d \n",from->tid,to->tid );
	ucontext_t *uc_link;
	alarm(1);

	// _swtch((void*)from,(void*)to);
	
}

//http://man7.org/linux/man-pages/man2/sigaltstack.2.html