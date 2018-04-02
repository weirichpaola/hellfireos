#include <hellfire.h>

#define N_USUARIOS	20
#define N_POMBOS	1

sem_t mutex, enchendo, cheia, mutex;
int32_t contaPostIt = 0, n = 20, wc = 0, reads = 0, writes = 0;

void pombo(void){
	while(1){
		hf_semwait(&cheia);
		hf_semwait(&mutex);
		leva_mochila();
		contaPostIt = 0;
		int32_t i;	
		for (i =0; i<n; i++){
			hf_sempost(&enchendo);
		}
		hf_sempost(&mutex);
	}
}
void colaPostIt_na_mochila(int32_t i, int32_t id){
	printf("[ %d ] colando postit %d na mochila  \n", i, id);
}
void leva_mochila(void){
	printf("pombo levando mensagens na mochila \n");
}

void usuario(void){
	int32_t i;

	i = hf_selfid();
	while(1){
		hf_semwait(&enchendo);
		hf_semwait(&mutex);	
		colaPostIt_na_mochila(contaPostIt, i);
		contaPostIt++;
		if (contaPostIt == n){
			hf_sempost(&cheia);
		}
		
		hf_sempost(&mutex);
	}
}

void app_main(void){
	int32_t i;

	hf_seminit(&enchendo, 20);
	hf_seminit(&mutex, 1);	
	hf_seminit(&cheia, 0);

	

	for(i = 0; i < N_USUARIOS; i++)
		hf_spawn(usuario, 0, 0, 0, "usuario", 2048);
	for(i = 0; i < N_POMBOS; i++)
		hf_spawn(pombo, 0, 0, 0, "pombo", 2048);

}

