#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void deposit(int *sum, int *cancel, int *yens, int *coins, int *storedYens);
void nextAction(int *sum, int *cancel, int *yens, int *coins, int *storedYens, int *num);
void calculate(int sum, int *yens, int *coins, int *storedYens, int *num);
void inputCalculate(int sum, int *yens, int *coins, int *storedYens);
void handleEMoney(int *ic_money, int ic_value, int *num, int *cancel);
void handleCancel(int *yens, int *storedYens);
// void handleLast(char *yesNo);

int plusYens(int value, int *yens, int *coins, int *storedYens); //coins メモリ節約

//所持している紙幣や貨幣が0枚のときのError処理はしていません。
int main()
{
  int cash = 130, icValue = 124;
  int ic_money = 1000;
  int coins[] = {10, 50, 100, 500, 1000, 5000, 10000};
  int storedYens[] = {15, 3, 2, 1, 1, 1, 1};

  while (1)
  {
    int sum = 0;
    int num = 0; //購入枚数
    int cancel = 0;
    int yens[] = {0, 0, 0, 0, 0, 0, 0};
    char yesNo[4];

    char way[5];
    printf("支払い方法を入力してください。\ncashまたはicと入力してEnterを押してください。\n");
    printf("入力時に cancel と入力すると最初からやり直すことができます\n");
    printf("cash : 現金  ic : 電子マネー\n");

    scanf("%5s", way);

    if (strcmp(way, "cash") == 0)
    {
      //入金 1回実行
      deposit(&sum, &cancel, yens, coins, storedYens);
      printf("%d\n", cancel);
      if (cancel == 1)
      {
        handleCancel(yens, storedYens);
        continue;
      }
      //入金 何度か実行
      nextAction(&sum, &cancel, yens, coins, storedYens, &num);
      printf("%d\n", cancel);
      if (cancel == 1)
      {
        handleCancel(yens, storedYens);
        continue;
      }

      //投入金とお釣りの計算と金種とその枚数の計算
      calculate(sum, yens, coins, storedYens, &num);

      //終了判定
      //handlelast(yesNo);
      while (1)
      {
        printf("続けて取引しますか？ yes/no : ");
        scanf("%s", yesNo);
        if (strcmp(yesNo, "yes") == 0)
          break;
        else if (strcmp(yesNo, "no") == 0)
          break;
        else
          printf("yesまたはnoを入力してください。");
      }
      if (strcmp(yesNo, "no") == 0)
        break;
    }
    else if (strcmp(way, "ic") == 0)
    {
      handleEMoney(&ic_money, icValue, &num, &cancel);
      if (cancel == 1)
        continue;

      //続行判定
      while (1)
      {
        printf("続けて取引しますか？ yes/no : ");
        scanf("%s", yesNo);
        if (strcmp(yesNo, "yes") == 0)
          break;
        else if (strcmp(yesNo, "no") == 0)
          break;
        else
          printf("yesまたはnoを入力してください。");
      }
      if (strcmp(yesNo, "no") == 0)
        break;
    }
    else
    {
      printf("無効な値が入力されました。入力し直してください。\n");
      continue;
    }
  }

  return 0;
}

//初回入金
void deposit(int *sum, int *cancel, int *yens, int *coins, int *storedYens)
{
  char input[6];
  do
  {
    printf("数字を入力してお金を投入してください: ");
    scanf("%6s", input);
    printf("input: %s\n", input);
    if (strcmp(input, "cancel") == 0)
    {
      *cancel = 1;
      break;
    }
    //if (sum += input > )
    int value = atoi(input);
    int judge = plusYens(value, yens, coins, storedYens);
    if (judge == 0)
      continue; //貨幣でなければcontinue
    *sum += value;
  } while (input <= 0); //無効な文字を判定しつつ負の値も排除
}

//2回目以降の入金＋購入フェーズ
void nextAction(int *sum, int *cancel, int *yens, int *coins, int *storedYens, int *num)
{
  while (1)
  {
    char input[7];
    //char str[7];
    printf("現在の投入金額 : %d円\n", *sum);
    printf("切符を購入するには buy, \n最初からやり直す場合は cancel, \n続けて入金する場合は入金額を入力してください。:");
    scanf("%6s", input);
    if (strcmp(input, "buy") == 0)
    {
      //buyプロシージャ
      printf("購入枚数を指定してください：");
      //cancel判定。
      do
      {
        //そのまま文字列を入れることになりそうなので再利用は無理そうだが、書けてみる
        scanf("%s", input);
        if (strcmp(input, "cancel") == 0)
        {
          *cancel = 1;
          return;
        }
        *num = atoi(input); //数字以外は0をthrowしてcatchするはず。
        if (*num <= 0)
          printf("枚数は数字で入力してください。\n");
      } while (*num <= 0);

      //scanf("%d", num);
      if (*sum < *num * 130)
      {
        printf("切符は%d円で買うことができます。\n", *num * 130);
        continue;
      }
      break; //calculateへ
    }
    else if (strcmp(input, "cancel") == 0)
    {
      //最初から
      *cancel = 1;
      break;
    }
    else
    {
      //atoi 数字以外は0
      int value = atoi(input);
      if (value != 0)
      {
        //int judge = minusCoin(value, ten, f_ten, hund, f_hund, thous, f_thous, ten_thous);
        int judge = plusYens(value, yens, coins, storedYens);
        if (judge == 1)
          *sum += value;
      }
    }
  }
}

//入金した金種と枚数増
int plusYens(int value, int *yens, int *coins, int *storedYens)
{
  if (value == 0) //valueがatoi で0または普通に0が来たとき用
  {
    printf("無効な値です\n");
    return 0;
  }
  else
  {
    int i = 0;
    while (i < 7)
    {
      if (value == coins[i])
      {
        if (storedYens[i] == 0)
        {
          printf("%d円が不足しています。\n", coins[i]);
          return 0;
        }
        yens[i] += 1;
        storedYens[i] -= 1;
        //printf("storedYens ￥%d\n", storedYens[i]);
        return 1;
      }
      i++;
    }
    printf("存在しない貨幣です。\n");
    return 0;
  }
}

//投入した金種と枚数を表示するメソッドを呼ぶ
//お釣りと手持ちの金種と枚数を表示
void calculate(int sum, int *yens, int *coins, int *storedYens, int *num)
{
  if (sum == *num * 130)
  {
    //投入した金額の計算と出力 //お釣り
    inputCalculate(sum, yens, coins, storedYens);
    //釣りなし→手元金だけ出力
    printf("お釣りはありません。\n手元にある金種は\n");
    for (int i = 0; i < 7; i++)
      printf("%d円が%d枚\n", coins[i], storedYens[i]);
  }
  else
  {
    int change = sum - *num * 130;
    inputCalculate(sum, yens, coins, storedYens);
    //釣
    printf("お釣りは\n");
    //手元金　釣り＋元金の枚数
    for (int i = 6; i >= 0; i--)
    {
      if (change / coins[i] >= 1)
      {
        storedYens[i] += change / coins[i];
        printf("%d円 : %d枚\n", coins[i], change / coins[i]);
        change -= coins[i] * (change / coins[i]);
      }
    }
    printf("手元にある金種は\n");
    for (int i = 0; i < 7; i++)
      printf("%d円が%d枚\n", coins[i], storedYens[i]);
    //break;
  }
}
//投入金額 表示
void inputCalculate(int sum, int *yens, int *coins, int *storedYens)
{
  for (int i = 0; i < 7; i++)
  {

    if (yens[i] != 0)
    {
      //払った金額
      printf("投入した金種は%d円が%d枚\n", coins[i], yens[i]);

      //使ったお金yens[i] 釣り銭関数 残金関数：釣り＋手持ち
    }
  }
}
//電子マネーの対応 雑な作りとなっている。
void handleEMoney(int *ic_money, int ic_value, int *num, int *cancel)
{
  int payment = 0;
  char input[7];
  printf("残高 : %d円\n", *ic_money);
  printf("購入枚数を指定してください：");
  do
  {
    scanf("%s", input);
    if (strcmp(input, "cancel") == 0)
    {
      *cancel = 1;
      return;
    }
    *num = atoi(input);
    if (*num < 1)
      printf("切符は1枚から購入可能です。\n再入力してください：");
  } while (*num < 1);

  payment = *num * ic_value;
  if (*ic_money - payment < 0)
  {
    printf("引去失敗\n引去額 : %d円\n残高 : %d円\n\n", payment, *ic_money);
    *num = 0;
    handleEMoney(ic_money, ic_value, num, cancel);
    return;
  }
  else
  {
    *ic_money -= payment;
    printf("引去成功\n引去額 : %d\n", payment);
    printf("購入が完了しました。\n残高 : %d円\n", *ic_money);
  }
}

void handleCancel(int *yens, int *storedYens)
{
  for (int i = 0; i < 7; i++)
  {
    if (yens[i] != 0)
      storedYens[i] += yens[i];
  }
}
