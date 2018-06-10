
typedef struct{
  int tokindex;
  int objectindex;
} NameTokenInfo;

typedef enum{
  NOBRAND = 0,
  ORION = 1,
  HAETAE = 2,
  NONGSIM = 3
} company_t;

typedef struct {
  company_t company;
  char name[20];
  int price;
  int count;
} product_t;
