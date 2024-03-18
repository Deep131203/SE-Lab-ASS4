#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

struct Inventory{
    int id;
    char productname[50];
    int quantity;
    float price;
    char date[12];
} iv;

FILE *fp;

void addproduct() {
    char myDate[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(myDate, "%02d/%02d/%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
    strcpy(iv.date, myDate);

    fp = fopen("product.txt", "ab");
    printf("Enter product id: ");
    scanf("%d", &iv.id);
    printf("Enter the product name: ");
    fflush(stdin);
    scanf("%s", &iv.productname);
    printf("Enter product quantity: ");
    fflush(stdin);
    scanf("%d", &iv.quantity);
    printf("Enter the product price: ");
    fflush(stdin);
    scanf("%f", &iv.price);
    printf("\nProduct added successfully...\n");
    fwrite(&iv, sizeof(iv), 1, fp);
    fclose(fp);
}

void displayproducts() {
    system("cls");
    printf("<=== Product List ===>\n\n");
    printf("%-10s %-30s %-15s %-15s %s\n", "Id", "Product Name", "Quantity", "Price", "Date");
    printf("-----------------------------------------------------------------\n");
    fp = fopen("product.txt", "rb");
    while (fread(&iv, sizeof(iv), 1, fp) == 1) {
        printf("%-10d %-30s %-15d %-15f %s\n", iv.id, iv.productname, iv.quantity, iv.price, iv.date);
    }
    fclose(fp);
}

void updateinventory() {
    int id, f;
    system("cls");
    printf("<== Update products ==>\n\n");
    printf("Enter the product id to update: ");
    scanf("%d", &id);
    FILE *ft;
    fp = fopen("product.txt", "rb+");
    while (fread(&iv, sizeof(iv), 1, fp) == 1) {
        if (id == iv.id) {
            f = 1;
            printf("Select the operation to be performed\n");
            printf("1. Update the product name\n");
            printf("2. Update the quantity\n");
            printf("3. Update the product price\n");
            int val;
            printf("Enter your choice: ");
            scanf("%d", &val);
            switch (val) {
                case 1:
                    printf("Enter the product name: ");
                    fflush(stdin);
                    scanf("%s", &iv.productname);
                    break;
                case 2:
                    printf("Enter product quantity: ");
                    fflush(stdin);
                    scanf("%d", &iv.quantity);
                    break;
                case 3:
                    printf("Enter the product price: ");
                    fflush(stdin);
                    scanf("%f", &iv.price);
                    break;
                default:
                    printf("Invalid input\n");
            }
            fseek(fp, -sizeof(iv), 1);
            fwrite(&iv, sizeof(iv), 1, fp);
            fclose(fp);
            break;
        }
    }
    if (f == 1) {
        printf("\nProduct updated...\n");
    } else {
        printf("\nProduct Not Found !!");
    }
}

void del(int id) {
    int f = 0;
    FILE *ft;
    fp = fopen("product.txt", "rb");
    ft = fopen("temp.txt", "wb");
    while (fread(&iv, sizeof(iv), 1, fp) == 1) {
        if (id == iv.id) {
            f = 1;
        } else {
            fwrite(&iv, sizeof(iv), 1, ft);
        }
    }
    fclose(fp);
    fclose(ft);
    remove("product.txt");
    rename("temp.txt", "product.txt");
}

void deleteproduct() {
    int id, f;
    system("cls");
    printf("<== Delete Products ==>\n\n");
    printf("Enter the product id to delete: ");
    scanf("%d", &id);
    FILE *ft;
    fp = fopen("product.txt", "rb");
    while (fread(&iv, sizeof(iv), 1, fp) == 1) {
        if (id == iv.id) {
            f = 1;
            fclose(fp);
            break;
        }
    }
    if (f == 1) {
        printf("Product deleted successfully...\n");
        del(id);
    } else {
        printf("\nProduct Not Found !!");
    }
}

void administrator() {
    int val;
    printf("\n");
    printf("1. Add product\n");
    printf("2. Update inventory\n");
    printf("3. Delete product\n");
    printf("4. Display products\n");

    scanf("%d", &val);
    switch (val) {
        case 1:
            addproduct();
            break;
        case 2:
            updateinventory();
            break;
        case 3:
            deleteproduct();
            break;
        case 4:
            displayproducts();
            break;
        default:
            printf("Invalid input\n");
    }
}

void buy() {
    int id, f = 0, quant;
    system("cls");
    printf("<== Buy products ==>\n\n");
    printf("Enter the product id to buy: ");
    scanf("%d", &id);
    printf("Enter the quantity of the product: ");
    scanf("%d", &quant);
    FILE *ft;
    float p;
    fp = fopen("product.txt", "rb+");
    while (fread(&iv, sizeof(iv), 1, fp) == 1) {
        if (id == iv.id) {
            p = iv.price;
            if (iv.quantity - quant < 0) {
                printf("Insufficient quantity available\n");
                return;
            } else if (iv.quantity - quant >= 0) {
                f = 1;
                iv.quantity = iv.quantity - quant;
                fseek(fp, -sizeof(iv), 1);
                fwrite(&iv, sizeof(iv), 1, fp);
                fclose(fp);
                if (iv.quantity == 0) {
                    del(iv.id);
                }
                break;
            }
        }
    }
    if (f == 1) {
        printf("<===== Here is the invoice =====>\n");
        printf("Total amount payable: %f\n", p * quant);
        printf("Product bought successfully...\n");
    } else {
        printf("Product Not Found !!\n");
    }
}

void customer() {
    int val;
    printf("1. Buy product\n");
    printf("2. View product inventory\n");
    printf("Enter the choice: ");
    scanf("%d", &val);
    switch (val) {
        case 1:
            buy();
            break;
        case 2:
            displayproducts();
            break;
        default:
            printf("Invalid input\n");
    }
}

int main() {
    int val;
    while (1) {
        printf("1. Administator\n");
        printf("2. Customer\n");
        printf("0. Exit\n");
        printf("-->Enter your choice: ");
        scanf("%d", &val);
        switch (val) {
            case 1:
                administrator();
                break;
            case 2:
                customer();
                break;
            case 0:
                exit(0);
            default:
                printf("Invalid input\n");
        }
    }
    return 0;
}
 
