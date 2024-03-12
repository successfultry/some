#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Order{
     int ID;
     int Qty;
     float Price;
     struct Order *next;
};

struct Order *Make_order_structure(void){
    struct Order *new_node;

    new_node = (struct Order*)malloc(sizeof(struct Order));
    if(new_node == NULL){
        puts("Error function malloc");
        exit (1);
    }

    return new_node;
};

///////Adding the request to the list of Buyers///////
void Add_in_list(struct Order *ptr, struct Order** dest, bool Buyer){
    if(Buyer){
        if(*dest == NULL){                       // Adding the Buyer's first request
            *dest = ptr;
        }
        else{
            if((*dest)->Price < ptr->Price){     // Adding subsequent Buyer's requests
                ptr->next = *dest;               // sort in descending order and Id
                *dest = ptr;
            }
            else{
                struct Order  *tmp  = (*dest)->next;
                struct Order *prev = *dest;
                while(tmp != NULL && ptr->Price  <= tmp->Price){
                        prev = tmp;
                        tmp  = tmp->next;
                }
                prev->next = ptr;
                ptr->next  = tmp;
            }
        }
    }
    ///////Adding a request to the Sellers list///////
    else{
        if(*dest == NULL){                       // Adding the Seller's first request
        *dest = ptr;
        }
        else{
            if((*dest)->Price > ptr->Price){     // Adding subsequent Seller's requests
                ptr->next = *dest;               // sort in ascending order and Id
                *dest = ptr;
            }
             else{
                struct Order *tmp  = (*dest)->next;
                struct Order *prev = *dest;
                while(tmp != NULL && ptr->Price  >= tmp->Price){
                        prev = tmp;
                        tmp  = tmp->next;
                }
                prev->next = ptr;
                ptr->next  = tmp;
            }
        }
    }
}

void Cancel_order(int id, struct Order** Buyer){

    struct Order  *tmp  = *Buyer;
    struct Order *prev  = *Buyer;

    if(*Buyer != NULL && ((*Buyer)->ID == id)){                   //Deleting the first application in the list
        *Buyer = (*Buyer)->next;
        free(prev);
        printf("X,%d\n",id);
    }

    else{
        while(tmp != NULL && tmp->ID != id){  //Deleting subsequent applications
            prev = tmp;                       //in the list
            tmp  = tmp->next;
        }
        if(tmp != NULL){
            prev->next = tmp->next;
            free(tmp);
            printf("X,%d\n",id);
        }
      }
}

void Trade(struct Order** Buyer, struct Order** Seller){

     static int trade_id = 0;

     while((*Buyer) != NULL  &&  (*Seller) != NULL  &&  (*Buyer)->Price >= (*Seller)->Price){

        int trade_volume = (*Buyer)->Qty > (*Seller)->Qty ? (*Seller)->Qty : (*Buyer)->Qty;   //specify the smallest volume for the transaction
        float trade_price = (*Buyer)->ID > (*Seller)->ID ? (*Seller)->Price : (*Buyer)->Price;//specify the price for the transaction

         if((*Buyer)->ID > (*Seller)->ID ){                                                   //We form a deal depending on ID
             printf("T,%d,S,%d,%d,%d,%.2f\n", ++trade_id, (*Seller)->ID,                      
                                          (*Buyer)->ID, trade_volume,
                                           trade_price);
        }

        else{
             printf("T,%d,B,%d,%d,%d,%.2f\n", ++trade_id, (*Buyer)->ID,
                                          (*Seller)->ID, trade_volume,
                                          trade_price);
        }


        (*Buyer)->Qty  -= trade_volume;
        (*Seller)->Qty -= trade_volume;


        if((*Buyer)->Qty == 0){                                                               //We remove the head from the list of Buyers
            struct Order  *tmp  = (*Buyer);                                                   //if the number of kg of the transaction is greater than in
            *Buyer = (*Buyer)->next;                                                          //a certain Buyer's request
            free(tmp);
        }

        if((*Seller)->Qty == 0){                                                             //We remove the head from the list of Sellers
            struct Order  *tmp  = (*Seller);                                                 //if the number of kg of the transaction is greater than in
            *Seller = (*Seller)->next;                                                       //a certain Seller's request
            free(tmp);
        }
     }
}

int main(){

    struct Order *Buy_list  = NULL;
    struct Order *Sell_list = NULL;

    char ch;
    char tmp_for_Side;
    bool Buyer;
    int Id_to_cancel;

    while((ch = getchar()) != EOF){

/////////Filling the structure with data/////////
        if(ch == 'O'){
           getchar();                            //skip the 1st comma

           struct Order* order = Make_order_structure();

           scanf("%d",&order->ID);
           getchar();                              //skip the 2nd comma
           tmp_for_Side = getchar();
           getchar();                              //skip the 3td comma
           scanf("%d",&order->Qty);
           getchar();                              //skip the 4th comma
           scanf("%f",&order->Price);
           order->next = NULL;

           if(tmp_for_Side == 'B'){                // Choosing a list of Sellers or Buyers
                Buyer = true;
                Add_in_list(order, &Buy_list, Buyer);
           }
           else{
                Buyer = false;
                Add_in_list(order, &Sell_list, Buyer);
           }


/////////Making the deal////////////
           Trade(&Buy_list, &Sell_list);


        }
/////////////Deleting the request////////////
        else if(ch == 'C'){
            getchar();                              //skip the 1st comma
            scanf("%d",&Id_to_cancel);
            Cancel_order(Id_to_cancel, &Buy_list);  //Looking for an order to be deleted
            Cancel_order(Id_to_cancel, &Sell_list); //in both lists

        }

 }

    return 0;
}
