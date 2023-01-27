#ifndef terminal_h
#define terminal_h
#include <time.h>
uint8_t y[4] = { '0' };
uint8_t m[3] = { '0' };
uint8_t d[2] = { '0' };
unsigned short n[4] = { 0 };

typedef struct ST_terminalData_t
{
	float transAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
	OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT
}EN_terminalError_t;

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData);



EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	itoa((tm.tm_year + 1900), y, 10);
	itoa((tm.tm_mon + 101), m, 10);
	itoa(tm.tm_mday, d, 10);
	termData->transactionDate[0] = d[0]; termData->transactionDate[1] = d[1]; termData->transactionDate[2] = '/';
	termData->transactionDate[3] = m[1]; termData->transactionDate[4] = m[2]; termData->transactionDate[5] = '/';
	termData->transactionDate[6] = y[0]; termData->transactionDate[7] = y[1]; termData->transactionDate[10] = '\0';
	termData->transactionDate[8] = y[2]; termData->transactionDate[9] = y[3];

	//printf("\n%c\n", termData->transactionDate[8]);

	if ((termData->transactionDate[10] == '\0') && (termData->transactionDate[2] == '/') && (termData->transactionDate[5] == '/')) 
		return OK;
	else 
		return WRONG_DATE;
}


/*cases
10/22 true
10/21 false

*/
EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData) {
	if ((cardData.cardExpirationDate[3] > termData.transactionDate[8])
		|| ((cardData.cardExpirationDate[3] == termData.transactionDate[8])
			&& (cardData.cardExpirationDate[4] > termData.transactionDate[9]))
		|| ((cardData.cardExpirationDate[3] == termData.transactionDate[8])
			&& (cardData.cardExpirationDate[4] == termData.transactionDate[9])
			&& (cardData.cardExpirationDate[0] > termData.transactionDate[3]))
		|| ((cardData.cardExpirationDate[3] == termData.transactionDate[8])
			&& (cardData.cardExpirationDate[4] == termData.transactionDate[9])
			&& (cardData.cardExpirationDate[0] == termData.transactionDate[3])
			&& (cardData.cardExpirationDate[1] > termData.transactionDate[4]))) {
		return OK;
	}
	else
	{
		printf("\n\n\t\t\t\t       Declined Expired card");	
		return EXPIRED_CARD;
	}
}

/*
cases:
1000
0
-10
*/

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {
	printf("please,Enter transaction amount: ");
	scanf("%f", &termData->transAmount);
	if (termData->transAmount <= 0.00) {
		return INVALID_AMOUNT;
	}
	else {
		return OK;
	}
}


EN_terminalError_t setMaxAmount(ST_terminalData_t* termData) {
	termData->maxTransAmount = 10000.00;
	if (termData->maxTransAmount <= 0.00) {
		return INVALID_MAX_AMOUNT;
	}
	else {
		return OK;
	}

}

/*
cases :
5000
10000
10001
*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount <= termData->maxTransAmount) {
		return OK;
	}
	else {
		printf("\n\n\t\t\t\t   Declined Amount Exceeding Limit");
		return EXCEED_MAX_AMOUNT;
	}
}
/*

cases :
4417123456789113
1234567812345678
4417123456789618

1. Write down the credit card number:
4417123456789113
2. Starting from the first number, double every other digit.
4(x2) 4 1(x2) 7 1(x2) 2 3(x2) 4 5(x2) 6 7(x2) 8 9(x2) 1 1(x2) 3
The doubled numbers result in: 8 2 2 6 10 14 18 2
3. If the result of the doubling ends up with a two digits, then add those two digits together:
10 = 1+0 14= 1+4 18= 1+8
4. Add up all numbers: 8+4+2+7 + 2+2+6+4 + 1+0+6+1+4+8 + 1+8+1+2+3 = 70
If the final sum is divisible by 10, 
then the credit card is valid. If it is not divisible by 10, the number is invalid or fake. 
In the above example, credit card number 4417 1234 5678 9113 has passed the Luhn test.*/

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData) {
	for (int h = 1; h < 16; h = h + 2) {
		if (cardData->primaryAccountNumber[h] == '0') { n[0] = 0; }
		else if (cardData->primaryAccountNumber[h] == '1') { n[0] = 1; }
		else if (cardData->primaryAccountNumber[h] == '2') { n[0] = 2; }
		else if (cardData->primaryAccountNumber[h] == '3') { n[0] = 3; }
		else if (cardData->primaryAccountNumber[h] == '4') { n[0] = 4; }
		else if (cardData->primaryAccountNumber[h] == '5') { n[0] = 5; }
		else if (cardData->primaryAccountNumber[h] == '6') { n[0] = 6; }
		else if (cardData->primaryAccountNumber[h] == '7') { n[0] = 7; }
		else if (cardData->primaryAccountNumber[h] == '8') { n[0] = 8; }
		else if (cardData->primaryAccountNumber[h] == '9') { n[0] = 9; }
		n[1] = n[0] + n[1];
	}

	for (int x = 0; x < 15; x = x + 2) {
		if (cardData->primaryAccountNumber[x] == '0') { n[0] = 0; }
		else if (cardData->primaryAccountNumber[x] == '1') { n[2] = 1; }
		else if (cardData->primaryAccountNumber[x] == '2') { n[2] = 2; }
		else if (cardData->primaryAccountNumber[x] == '3') { n[2] = 3; }
		else if (cardData->primaryAccountNumber[x] == '4') { n[2] = 4; }
		else if (cardData->primaryAccountNumber[x] == '5') { n[2] = 5; }
		else if (cardData->primaryAccountNumber[x] == '6') { n[2] = 6; }
		else if (cardData->primaryAccountNumber[x] == '7') { n[2] = 7; }
		else if (cardData->primaryAccountNumber[x] == '8') { n[2] = 8; }
		else if (cardData->primaryAccountNumber[x] == '9') { n[2] = 9; }
		if (n[2] >= 5) {
			n[2] = (2 * n[2]) - 9;
		}
		else {
			n[2] = n[2] * 2;
		}
		n[3] = n[2] + n[3];
	}
	if ((n[1] + n[3]) % 10 == 0) {
		return OK;
	}
	else {
		return INVALID_CARD;
	}
}

#endif