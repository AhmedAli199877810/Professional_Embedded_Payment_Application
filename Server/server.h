#ifndef server_h
#define server_h
uint32_t g[14] = { 0 };

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, INTERNAL_SERVER_ERROR
}EN_transStat_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transStat_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction;

typedef struct ST_accountsDB_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountsDB_t;

typedef enum EN_serverError_t
{
	oK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, No
}EN_serverError_t;



ST_transaction get_struct; //structure for get transaction
ST_transaction tran_array[255] = { 0 };
ST_accountsDB_t valid_accounts[255] = { {25000.00,"4417123456789113"} };

EN_transStat_t recieveTransactionData(ST_transaction* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction* transData);

/*
cases:
- error cardit data
- error terminal data
- error expired data
*/

EN_serverError_t saveTransaction(ST_transaction* transData){
		static int p = 0;
		g[10] = isValidAccount(&transData->cardHolderData);
		g[4] = getTransactionDate(&transData->terminalData);
		g[5] = isCardExpired(transData->cardHolderData , transData->terminalData);
		if (g[5] != 0)
		{
			_Exit(0);
		}
		else if (g[10] != 0)
		{
			printf("\n\n\t\t\t\t      Declined Invalid Account\n");
			_Exit(0);
		}
		g[11] = isAmountAvailable(&transData->terminalData);
		if (g[11] == 0 ) {
			tran_array[p].cardHolderData = transData->cardHolderData;
			tran_array[p].terminalData = transData->terminalData;
			tran_array[p].transactionSequenceNumber = transData->transactionSequenceNumber = p =g[13];
			p++;
			return oK;
		}
		_Exit(0);
		return SAVING_FAILED;
	}
/*
cases:
- error name
- error syntx pan
- error luhn pan   
- error syntX expired data
- error account  
4417123456789113 true
4417123456789816 false
*/
EN_serverError_t isValidAccount(ST_cardData_t* cardData) {
	g[0] = getCardHolderName(cardData);
	g[1] = getCardPAN(cardData);
	g[2] = getCardExpiryDate(cardData);
	g[3] = isValidCardPAN(cardData);
		if (g[0] == 0 && g[1] == 0 && g[2] == 0 && g[3] == 0) {
			for (int x = 0; x < 255; x++) {
				if (strcmp(cardData->primaryAccountNumber, valid_accounts[x].primaryAccountNumber) == 0) {
					g[9] = x;
					return oK; //0
					_Exit(0);
				}
			}
			return ACCOUNT_NOT_FOUND; //3
		}
	return No; //5
}

/*
cases:
- error syntX  data
- error max amount
- error transaction amount
- error below max amount
- error balance 
5000 false
2000 true
*/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData){
	g[6] = setMaxAmount(termData);
	g[7] = getTransactionAmount(termData);
	g[8] = isBelowMaxAmount(termData);
	if (g[8] != 0)
	{
		_Exit(0);
	}
	if (g[6] == 0 && g[7] == 0 &&g[8] ==0) {
		if (termData->transAmount <= valid_accounts[g[9]].balance) {
			return oK;  //0
		}

		printf("\n\n\t\t\t\t     Declined Insuffecient Funds\n");
		return LOW_BALANCE;     //4
	}
	return No; //5
}
/*
cases: false
- error cardit data    
- error valid account 1234567891234564

- error terminal data  
- error balance 

- error save data 
*/

EN_transStat_t recieveTransactionData(ST_transaction * transData) {
		g[12] = saveTransaction(transData);
		if (g[10] != 0 || g[10] == 5) {
			return DECLINED_STOLEN_CARD;  
		}
		else if (g[11] != 0 || g[11] == 5) {
			return DECLINED_INSUFFECIENT_FUND;
		}
		else if (g[12] != 0) {
			return INTERNAL_SERVER_ERROR;
		}
		else
		{
			valid_accounts[g[9]].balance = valid_accounts[g[9]].balance - transData->terminalData.transAmount;
			printf("\ncurrent balance = %f ", valid_accounts[g[9]].balance);
			printf("\n\n\t\t\t\t\t      Thank\n");
			return  APPROVED;
		}
	}

/*
cases:
- error sequancenumber
*/
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction* transData) {
	for (int x = 0; x < 255; x++) {
		if (transactionSequenceNumber == transData[x].transactionSequenceNumber) {
			get_struct = transData[x];
			return oK;
			_Exit(0);
		}
		return TRANSACTION_NOT_FOUND;
	}
}


#endif