#include "keelog_func.h"
//#include "stm32f1xx_hal.h"
#include "433Modem.h"


# define KeeLoq_NLF		0x3A5C742E
# define bit(x,n)		(((x)>>(n))&1)
# define g5(x,a,b,c,d,e)	(bit(x,a)+bit(x,b)*2+bit(x,c)*4+bit(x,d)*8+bit(x,e)*16)



uint32_t KeeLoq_Encrypt (const uint32_t data, const uint64_t key){
	uint32_t	x = data, r;
	for (r = 0; r < 528; r++)
		x = (x>>1)^((bit(x,0)^bit(x,16)^(uint32_t)bit(key,r&63)^bit(KeeLoq_NLF,g5(x,1,9,20,26,31)))<<31);
	return x;
}

uint32_t KeeLoq_Decrypt (const uint32_t data, const uint64_t key){
	uint32_t	x = data, r;
	for (r = 0; r < 528; r++)
		x = (x<<1)^bit(x,31)^bit(x,15)^(uint32_t)bit(key,(15-r)&63)^bit(KeeLoq_NLF,g5(x,0,8,19,25,30));
	return x;
}


uint64_t Normal_Learning (uint32_t data, const uint64_t key){ //data -serial nomber (28bit) key - manufacture (64bit)
	
uint32_t k1,k2;
	
data&=0x0FFFFFFF;
data|=0x20000000;
k1=KeeLoq_Decrypt(data, key);

data&=0x0FFFFFFF;
data|=0x60000000;
k2=KeeLoq_Decrypt(data, key);

return ((uint64_t)k2<<32)| k1; //key - shifrovanoya
}




uint64_t Reverse_BIT (uint64_t data){
	uint64_t reverse_data;	
	for (uint8_t i=0;i<64;i++){ 
	reverse_data = reverse_data<<1|bitRead(data, i);	
	}
return reverse_data;	
}

