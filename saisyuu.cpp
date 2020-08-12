#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <time.h>
#include <math.h>
using namespace std;

/*マス目の構造体を定義*/
struct Grid {
  //bool型で色を決定,trueなら有色,falseなら無色
  bool yourColor = false; //現在のマス目の色
  bool collectColor = false; //答えの色
  void changeYourColor(); //マス目の色を変更
  bool checkYourAns(); // そのマス目について正解か確認
};

void Grid::changeYourColor() {
  yourColor = !yourColor; //そのマス目の色を変更
}

bool Grid::checkYourAns() {
  /*プレイヤの色と答えが合っているか確認*/
  if (yourColor == collectColor) return true;
  return false;
}


/*問題を生成*/
vector<vector<Grid>> makeAns(int hight, int width) {
  vector<vector<Grid>> ans(hight, vector<Grid>(width)); //Grid型のvectorを定義
  int random; //乱数を生成

  /*乱数で各マス目の色(答え)を決定*/
  for (int y = 0; y < hight; y++) {
    static clock_t timPre, timCur = clock();
    srand((unsigned int)timCur); /*時間で初期化*/
    for (auto y = ans.begin(); y != ans.end(); y++) {
      for (auto x = (*y).begin(); x != (*y).end(); x++) {
        random = rand() % 2; //0または1の乱数を生成
        if (random == 1) {
          (*x).collectColor = true; //0なら無色に設定
        }else {
          (*x).collectColor = false; //1なら有色(黒)に設定
        }
      }
    }
    /*同じパターンの乱数を生成を防ぐ_処理間隔を開ける*/
    timPre = timCur;
    while ((unsigned int)timPre == (unsigned int)timCur) {
      timCur = clock();
    }
  }
  return ans;
}


/*行ヒント情報の取得*/
vector<vector<int>> getHintLine(vector<vector<Grid>> ans, int hight, int width){
  vector<vector<int>> hintLine(hight, vector<int>()); //行のヒントを格納するためのvectorを生成
  for (int i = 0; i < hight; i++) {
    static int count = 0;
    for (int j = 0; j < width; j++) {
      if (ans.at(i).at(j).collectColor) {
        count++;
      }
      if (!ans.at(i).at(j).collectColor && count != 0) {
        hintLine[i].push_back(count); //黒いマス目が連続する回数を代入
        count = 0;
      }
    }
    if (count != 0) {
      hintLine[i].push_back(count);
    }
    if (hintLine.at(i).empty()) {
      hintLine[i].push_back(0); //その列に黒いマス目が1つもない場合は0を代入
    }
    count = 0;
  }
  return hintLine;
}


/*列ヒント列情報の取得*/
vector<vector<int>> getHintRaw(vector<vector<Grid>> ans, int hight, int width){
  vector<vector<int>> hintRaw(width, vector<int>());
  for (int x = 0; x < width; x++) {
    static int count = 0;
    for (int y = 0; y < hight; y++) {
      if (ans.at(y).at(x).collectColor) {
        count++;
      }
      if (!ans.at(y).at(x).collectColor && count != 0) {
        hintRaw[x].push_back(count); //黒いマス目の連続回数を代入
        count = 0;
      }
    }
    if (count != 0) {
      hintRaw[x].push_back(count);
    }
    if (hintRaw.at(x).empty()) { //その列に黒いマス目が1つもない場合は0を代入
      hintRaw[x].push_back(0);
    }
    count = 0;
  }
  return hintRaw;
}



/*問題と別問題の解が同じになるか確認*/
bool isSameAns(vector<vector<Grid>> anoSol, vector<vector<int>>hintLOrg, vector<vector<int>>hintROrg) {
  int hight, width;
  hight = end(anoSol) - begin(anoSol);
  width = end(anoSol.at(0)) - begin(anoSol.at(0));
  vector<vector<int>> hintL, hintR;
  hintL = getHintLine(anoSol, hight, width); //別解候補の行ヒント情報を調査
  hintR = getHintRaw(anoSol, hight, width); //別解の列ヒント情報を調査
  if (hintL == hintLOrg && hintR == hintROrg) {
    return true;
  }
  return false;
}


/*組み合わせ計算*/
void combination(const vector<int>& color, vector<int>& result, vector<int>& box, unsigned n) {
  if (result.size() == n) { //指定選択数まで探索したか判断
    for (auto& i : result) {
      box.push_back(i); //組み合わせ情報を記録
    }
  }
  else {
    vector<int>::const_iterator i;
    if (result.empty()) {
      i = color.cbegin();
    }
    else {
      i = find(color.cbegin(), color.cend(), result.back()) + 1;
    }
    while (i != color.end()) {
      result.push_back(*i);
      combination(color, result, box, n);
      result.pop_back();
      i++;
    }
  }
}



/*全要素がfalseから成る2次元vectorの生成*/
vector<vector<Grid>> resetV(int hight, int width) {
  vector<vector<Grid>> v(hight, vector<Grid>(width));
  for (auto i = v.begin(); i != v.end(); i++) {
    for (auto j = (*i).begin(); j != (*i).end(); j++) {
        (*j).collectColor = false;
    }
  }
  return v;
}






/*作成した問題の一意性を確認*/
bool checkAnoSol(int hight, int width, vector<vector<int>>hintLOrg, vector<vector<int>>hintROrg) {
  vector<vector<Grid>> anoSol; //別解探索用
  vector<int> choice, result, comb; //マス目の数分の数値格納用,組み合わせの一時保持用,組み合わせ全記録用
  int solutions = 0, numcolor = 0, combElements = 0, numElements; //解の個数,黒いマス目の合計,組み合わせ格納用,マス目の合計数
  numElements = hight * width; //マス目の合計数を計算
  /*模範解答の黒いマス目の合計数を調査*/
  for (auto i = hintLOrg.begin(); i != hintLOrg.end(); i++) {
    for (auto j = (*i).begin(); j != (*i).end(); j++) {
        numcolor += *j;
    }
  }
  /*マス目の合計数と同じだけのvectorを生成*/
  for (int i = 0; i < numElements; i++) {
    choice.push_back(i);
  }
  combination(choice, result, comb, numcolor); //色を塗るマス目の組合せを調査
  for (auto i = comb.begin(); i != comb.end(); i++) {
    combElements++;
  }
  /*別解探索*/
  for (int i = 0; i < combElements / numcolor; i++) {
    anoSol = resetV(hight, width);
    for (int j = 0; j < numcolor; j++) { //numcolorは有色マスの個数
      anoSol.at(comb[i * numcolor + j] / width).at(comb[i * numcolor + j] % width).collectColor = true; //指定した行,列の色を黒へ
    }
    if (isSameAns(anoSol, hintLOrg, hintROrg)) solutions++; //解が同じになるか確認
    if(1 < solutions) return false; //複数解が出たらアウト
  }
  if (solutions == 1) return true; //解が1つならOK
  else return false;
}




/*1行分のヒント情報表示*/
void printHintL(int y, vector<vector<int>> hintLine) {
  int sizeVecL;
  cout << (" ");
  if (!hintLine.at(y).empty()) {
    sizeVecL = end(hintLine.at(y)) - begin(hintLine.at(y));
    for (int i = 0; i < sizeVecL; i++) {
      char num[3] = ("０"); //全角文字表示用
      num[1] = num[1] + hintLine.at(y).at(i); //全角に変換
      cout << num;
    }
  }
}


/*全部列分のヒントを表示*/
void printHintR(vector<vector<int>> hintRaw) {
  int sizeVecL, maxSizeVecL = 0, sizeVecR; //列ヒントvecの行数,最大行数,列数
  sizeVecR = end(hintRaw) - begin(hintRaw);
  /*列ヒントvecctorの最大行数を調査*/
  for (int i = 0; i < sizeVecR; i++) {
    sizeVecL = end(hintRaw.at(i)) - begin(hintRaw.at(i));
    if (maxSizeVecL < sizeVecL) {
      maxSizeVecL = sizeVecL;
    }
  }
  /*列ヒントを全て表示*/
  for (int i = 0; i < maxSizeVecL; i++) {
    cout << ("　");
    for (int j = 0; j < sizeVecR; j++) {
      sizeVecL = end(hintRaw.at(j)) - begin(hintRaw.at(j));
      if (i < sizeVecL) {
        char num[3] = ("０"); //全角変換用
        num[1] = num[1] + hintRaw.at(j).at(i); //全角文字として出力
        cout << num;
      } else {
        cout << ("　");
      }
    }
    cout << endl;
  }
}


/*プレイヤーの解答と模範解答が一致するか確認*/
bool checkYourAns(vector<vector<Grid>> grids) {
  for (auto y = grids.begin(); y != grids.end(); y++) {
    for (auto x = (*y).begin(); x != (*y).end(); x++) {
      if (!(*x).checkYourAns()) return false;
    }
  }
  return true;
}



/*描画時のマーカーの選択*/
void selectPatern(int y, int posY, int x, int posX, bool yourColor) {
  if ((x == posX) && (y == posY)) {// カーソルの位置を表示
    if (yourColor) {
      cout << "★";
    } else {
      cout << "☆";
    }
  }else { //マス目描画
    if (yourColor) {
      cout << "■"; //黒いマス目を表示
    } else {
      cout << "□"; //白いマス目を表示
    }
  }
}


/*描画処理*/
void drawPic(vector<vector<Grid>>& grids, int posY, int posX, vector<vector<int>> hintLine, vector<vector<int>> hintRaw) {
  cout << endl;
  for (size_t y = 0; y < grids.size(); y++) {
    cout << ("　");
    for (size_t x = 0; x < grids.at(y).size(); x++) {
      selectPatern(y, posY, x, posX, grids.at(y).at(x).yourColor); //マス目の模様を表示
    }
    printHintL(int(y), hintLine); //行ヒントの表示
    cout << endl;
  }
  printHintR(hintRaw); //最後に列ヒントの表示
}



/*入力処理*/
void input(vector<vector<Grid>>& grids, int &posY, int &posX, int hight, int width) {
  switch (_getch()) { //キーボード入力の受付
    case 'p': //pが入力されたらそのマス目の色を変える
      grids.at(posY).at(posX).changeYourColor();
      break;
    case 'u': //uが入力されたらカーソルを上に移動
      posY--;
      if (posY < 0) posY = 0; //移動制限
      break;
    case 'd': //dが入力されたらカーソルを下に移動
      posY++;
      if (hight <= posY) posY --; //移動制限
      break;
    case 'r':
      posX++; //rが入力されたらカーソルを右に移動
      if (width <= posX) posX --; //移動制限
      break;
    case 'l':
      posX--; //lが入力されたらカーソルを左に移動
      if (posX < 0) posX = 0; //移動制限
      break;
    default :
      break;
  }
}



int main() {
  const int hight = 4, width = 4; //マス目のサイズ_必ず1桁に設定
  vector<vector<Grid>> grids(hight, vector<Grid>(width)); //プレイヤーの解答情報を格納
  vector<vector<int>> hintLine; //行ヒント情報
  vector<vector<int>> hintRaw; //列ヒント情報

  /*問題生成処理*/
  cout << ("問題作成中");
  do {
    grids.clear(); //再度問題生成時にvectorの中身を消去
    hintLine.clear(); //再度問題生成時にvectorの中身を消去
    hintRaw.clear(); //再度問題生成時にvectorの中身を消去
    grids = makeAns(hight, width); //問題生成
    hintLine = getHintLine(grids, hight, width);
    hintRaw = getHintRaw(grids, hight, width);
  } while(!checkAnoSol(hight, width, hintLine, hintRaw)); //作問が適当か確認
  system("cls");

  /*描画処理*/
  while (!checkYourAns(grids)) {
    static int posX = 0, posY = 0; //カーソルの座標定義
    drawPic(grids, posY, posX, hintLine, hintRaw); //描画処理
    cout << endl << endl;
    input(grids, posY, posX, hight, width); //入力処理
    system("cls"); //画面消去
  }
  drawPic(grids, -1, -1, hintLine, hintRaw);
  cout << "正解!クリア!" << endl;


}
