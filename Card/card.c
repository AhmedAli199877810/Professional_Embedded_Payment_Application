#include "card.h"
/*
cases:
ahmedmohamedaliahme 19 false
ahmedmohamedaliahmed 20 true
ahmedmohamedaliahmedali 23 true
ahmedmohamedaliahmedalia 24 true
ahmedmohamedaliahmedaliai 25  false

*/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {
	printf("please,Enter card name : ");
	scanf("%s", &cardData->cardHolderName);
	for (unsigned int x = 0; x < 25; x++) {
		if (cardData->cardHolderName[x] == '\0' && x < 24) {
			flag[0] = 1;
		}
		if ((cardData->cardHolderName[x] == '\0' && x < 20) || (cardData->cardHolderName[x] != '\0' && x == 24 && flag[0] == 0)) {
			return WRONG_NAME;
			_Exit(0);
		}
	}
	return Ok;
}

/*
cases:
18/22 false
12/22 true
01/22 true
01/225 false
0122 false
01\22 false
*/
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {
	printf("please,Enter card date \"MM/YY\" : ");
	scanf("%s", &cardData->cardExpirationDate);
	if ((cardData->cardExpirationDate[5] == '\0') && (cardData->cardExpirationDate[2] == '/') &&
		(cardData->cardExpirationDate[0] < '1' || (cardData->cardExpirationDate[0] == '1' && cardData->cardExpirationDate[1] <= '2'))) {
		flag[1] = 1;
	}
	for (unsigned int x = 0; x < 6; x++) {
		if ((cardData->cardExpirationDate[x] == '\0' && x < 5) || (flag[1] == 0)) {
			return WRONG_EXP_DATE;
			_Exit(0);
		}
	}
	return Ok;
}

/*cases:
122234567891234 15 false
1222345678912345 16 true
12223456789123457 17 true
1222345678912345123 19 true
12223456789123451234 20 false
*/
EN_cardError_t getCardPAN(ST_cardData_t* cardData) {
	printf("please,Enter PAN : ");
	scanf("%s", &cardData->primaryAccountNumber);
	for (unsigned int x = 0; x < 20; x++) {
		if (cardData->primaryAccountNumber[x] == '\0' && x < 19) {
			flag[2] = 1;
		}
		if ((cardData->primaryAccountNumber[x] == '\0' && x < 16) || (cardData->primaryAccountNumber[x] != '\0' && x == 19 && flag[3] == 0)) {
			return WRONG_PAN;
			_Exit(0);
		}
	}
	return Ok;
}
