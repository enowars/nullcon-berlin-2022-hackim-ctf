#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char* int_to_str(mpz_t n) {
	int len = (int)mpz_sizeinbase(n, 256);
	char* res = malloc(len + 1);
	mpz_export(res, NULL, 1, 1, 0, 0, n);
	return res;
}
 
int main()
{
	char magic_words[12][6] = {"ene","mene","mike","moo","holy","moly","ekki","pteng","wuap","spam","ham","egg"};
	char* flag = "ENO{c0mp4re_th3_whol3_length}";

	int index;
	char target[256] = "";
	srand(time(0));
	for(int i = 0; i < 12; i++) {
		index = rand() % 12;
		strcat(target, magic_words[index]);
		strcat(target, " ");
	}
	strcat(target, "egg -- give me the flag!"); //end with "egg" to make sure it rhymes

	printf("You think you can give me orders? Prove your authority by signing:\n\"%s\"\nI want the signature as decimal number.\n", target);
	fflush(stdout);
	char inputStr[1025];
	scanf("%1024s" , inputStr);

	mpz_t n, signature, m;
	mpz_init_set_str(n,	"618017787734894212297412626251373685391953260761562256145235489725638613940584232503544382290786003502312209667905128601350448023284044287685001349014156076694914712630398005605673740514051697161500094865589407030636020089742702469018865286861672757837780829537694996694833864683805379159042221723723842387709902526369317372614408759564577441603426577837263761362135010436432956275299236838020581149454958441919492440492313018496438498099089383812883390835577605611991978206224568903757920872823293843687276713402767349404190867763874002307512458092607168715450826196573891917818134117192644486518276385456954928916750027466243826789735151096947239658610479195958319740857188397997117968901882796441973903340603204071299778964190138301428335590196310114790206400024512865068139543501059161433303153043130579702895687471084173256419199357102357728600646492441278405167508180066175830384059728198376079159561904247074846373084032155551839906550032001949969880315993549738076287202881585332049659570380033962200120029734918281696120171036422629762465758525540626266912510777059762980057239483211399081674862153728665111324068484754456207003656318475064950873727298779453666117120500091671142668282469471670064268176600698035510728754159", 0);
	mpz_init_set_str(signature,	inputStr, 0);
 
	mpz_init(m);
	mpz_powm_ui(m, signature, 3, n);

	char* message = int_to_str(m); 
	if(strcmp(message, target) == 0) {
		printf("You were right. Here is your flag:\n%s\n", flag);
		fflush(stdout);
	}
	else {
		printf("There is an impostor among us.\n");
		fflush(stdout);
	}

	mpz_clear(n);
	mpz_clear(signature);
	mpz_clear(m);
 
	return 0;
}
