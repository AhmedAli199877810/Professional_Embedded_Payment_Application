#ifndef app_h
#define app_h
ST_transaction w;
void appStart(void);
void appStart(void){
printf("\t\t\t\t\tPayment Application\n");
printf("\n\t\t\t\t\t     Welcome\n\n");
recieveTransactionData(&w);
}
#endif