#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <time.h>
#include <math.h>
using namespace std;

struct Grid {
  //bool型で色を決定,trueなら有色,falseなら無色
  bool colorNow = false; //現在のマス目の色
  bool collectColor = false; //答えの色
};


/*問題を生成試作 Grid型*/
vector<vector<Grid>> makeAns(int hight, int width) {
  vector<vector<Grid>> ans(hight, vector<Grid>(width));
  int random;
  /*乱数で各マス目の色(答え)を決定*/
  for (int y = 0; y < hight; y++) {
    static clock_t timPre, timCur = clock();
    srand((unsigned int)timCur); /*時間で初期化*/
    for (auto y = ans.begin(); y != ans.end(); y++) {
      for (auto x = (*y).begin(); x != (*y).end(); x++) {
        random = rand() % 2;
        if (random == 1) {
          (*x).collectColor = true;
        }else {
          (*x).collectColor = false;
        }
      }
    }
    /*同じパターンにならないように配慮_処理間隔を開ける*/
    timPre = timCur;
    while ((unsigned int)timPre == (unsigned int)timCur) {
      timCur = clock();
    }
  }
  return ans;
}


/*行ヒント情報の取得*/
vector<vector<int>> getHintLine(vector<vector<Grid>> ans, int hight, int width){
  vector<vector<int>> hintLine(hight, vector<int>());
  for (int i = 0; i < hight; i++) {
    static int count = 0;
    for (int j = 0; j < width; j++) {
      if (ans.at(i).at(j).collectColor) {
        count++;
      }
      if (!ans.at(i).at(j).collectColor && count != 0) {
        hintLine[i].push_back(count);
        count = 0;
      }
    }
    if (count != 0) {
      hintLine[i].push_back(count);
    }
    if (hintLine.at(i).empty()) {
      hintLine[i].push_back(0);
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
        hintRaw[x].push_back(count);
        count = 0;
      }
    }
    if (count != 0) {
      hintRaw[x].push_back(count);
    }
    if (hintRaw.at(x).empty()) {
      hintRaw[x].push_back(0);
    }
    count = 0;
  }
  return hintRaw;
}



/*AA同じ解があるか確認vecダミーv2答え*/
bool isSameAns(vector<vector<Grid>> vec, vector<vector<int>>hintLOrg, vector<vector<int>>hintROrg) {
  int hight, width;
  hight = end(vec) - begin(vec);
  width = end(vec.at(0)) - begin(vec.at(0));
  vector<vector<int>> hintL, hintR;
  hintL = getHintLine(vec, hight, width);
  hintR = getHintRaw(vec, hight, width);
  if (hintL == hintLOrg && hintR == hintROrg) {
    return true;
  }
  return false;
}

/**/
void combination(const vector<int>& color, vector<int>& result, vector<int>& box, unsigned n) {
  if (result.size() == n) {
    for (auto& i : result) {
      box.push_back(i);
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



/*全要素が0から成る2次元vectorの生成*/
vector<vector<Grid>> resetV(int hight, int width) {
  vector<vector<Grid>> v(hight, vector<Grid>(width));
  for (auto i = v.begin(); i != v.end(); i++) {
    for (auto j = (*i).begin(); j != (*i).end(); j++) {
        (*j).collectColor = false;
    }
  }
  return v;
}






/*問題の一意制*/
bool checkAnoSol(int hight, int width, vector<vector<int>>hintLOrg, vector<vector<int>>hintROrg) {
  vector<vector<Grid>> anoSol;
  vector<int> choice, result, comb;
  int solutions = 0, numcolor = 0, combElements = 0, numElements;
  for (auto i = hintLOrg.begin(); i != hintLOrg.end(); i++) {
    for (auto j = (*i).begin(); j != (*i).end(); j++) {
        numcolor += *j;
    }
  }
  numElements = hight * width;
  for (int i = 0; i < numElements; i++) {
    choice.push_back(i);
  }
  combination(choice, result, comb, numcolor);
  for (auto i = comb.begin(); i != comb.end(); i++) {
    combElements++;
  }
  for (int i = 0; i < combElements / numcolor; i++) {
    anoSol = resetV(hight, width);
    for (int j = 0; j < numcolor; j++) { //numcolorは有色マスの個数
      anoSol.at(comb[i * numcolor + j] / width).at(comb[i * numcolor + j] % width).collectColor = true;
    }
    if (isSameAns(anoSol, hintLOrg, hintROrg)) solutions++;
    if(1 < solutions) return false;
  }
  if (solutions == 1) return true;
  else return false;
}




/*1行分のヒント情報表示*/
void printHintL(int y, vector<vector<int>> hintLine) {
  int sizeVecL;
  cout << (" ");
  if (!hintLine.at(y).empty()) {
    sizeVecL = end(hintLine.at(y)) - begin(hintLine.at(y));
    for (int i = 0; i < sizeVecL; i++) {
      char num[3] = ("０");
      num[1] = num[1] + hintLine.at(y).at(i);
      cout << num;
    }
  }
}


/*全部列分のヒントを表示*/
void printHintR(vector<vector<int>> vec) {
  int sizeVecL, maxSizeVecL = 0, sizeVecR;
  sizeVecR = end(vec) - begin(vec);
  for (int i = 0; i < sizeVecR; i++) {
    sizeVecL = end(vec.at(i)) - begin(vec.at(i));
    if (maxSizeVecL < sizeVecL) {
      maxSizeVecL = sizeVecL;
    }
  }

  for (int i = 0; i < maxSizeVecL; i++) {
    cout << ("　");
    for (int j = 0; j < sizeVecR; j++) {
      sizeVecL = end(vec.at(j)) - begin(vec.at(j));
      if (i < sizeVecL) {
        char num[3] = ("０");
        num[1] = num[1] + vec.at(j).at(i);
        cout << num;
      } else {
        cout << ("　");
      }
    }
    cout << endl;
  }
}


/*v2に模範解答を代入*/
bool checkYourAns(vector<vector<Grid>> v1, vector<vector<Grid>> v2) {
  int i = 0, j = 0;
  for (auto y = v1.begin(); y != v1.end(); y++) {
    for (auto x = (*y).begin(); x != (*y).end(); x++) {
      bool a, b, isEqual;
      a = v1.at(i).at(j).colorNow;
      b = v2.at(i).at(j).collectColor;
      isEqual = (a == b);
      if (!isEqual) return false;
      j++;
    }
    j = 0;
    i++;
  }
  return true;
}

/*マーカーの選択*/
void selectPatern(int y, int posY, int x, int posX, bool yourColor) {
  if ((x == posX) && (y == posY)) {
    if (yourColor) {
      cout << "★";
    } else {
      cout << "☆";
    }
  }else {
    if (yourColor) {
      cout << "■";
    } else {
      cout << "□";
    }
  }
}

/*描画処理*/
void drawPic(vector<vector<Grid>>& vec, int posY, int posX, vector<vector<int>> hintLine, vector<vector<int>> hintRaw) {
  cout << endl;
  for (size_t y = 0; y < vec.size(); y++) {
    cout << ("　");
    for (size_t x = 0; x < vec.at(y).size(); x++) {
      selectPatern(y, posY, x, posX, vec.at(y).at(x).colorNow);
    }
    printHintL(int(y), hintLine);
    cout << endl;
  }
  printHintR(hintRaw);
}



void input(vector<vector<Grid>>& vec, int &posY, int &posX, int hight, int width) {
  switch (_getch()) {
    case 'p':
      vec.at(posY).at(posX).colorNow = !vec.at(posY).at(posX).colorNow;
      break;
    case 'u':
      posY--;
      if (posY < 0) posY = 0;
      break;
    case 'd':
      posY++;
      if (hight <= posY) posY --;
      break;
    case 'r':
      posX++;
      if (width <= posX) posX --;
      break;
    case 'l':
      posX--;
      if (posX < 0) posX = 0;
      break;
    default :
      break;
  }
}



int main() {
  const int hight = 4, width = 4;
  vector<vector<Grid>> yourAns(hight, vector<Grid>(width));
  vector<vector<Grid>> collectAns;
  vector<vector<int>> hintLine;
  vector<vector<int>> hintRaw;

  cout << ("問題作成中");
  do {
    collectAns.clear();
    hintLine.clear();
    hintRaw.clear();
    collectAns = makeAns(hight, width);
    hintLine = getHintLine(collectAns, hight, width);
    hintRaw = getHintRaw(collectAns, hight, width);
  } while(!checkAnoSol(hight, width, hintLine, hintRaw));
  system("cls");


  while (!checkYourAns(yourAns, collectAns)) {
    static int posX = 0, posY = 0;
    drawPic(yourAns, posY, posX, hintLine, hintRaw);
    cout << endl << endl;
    input(yourAns, posY, posX, hight, width);
    system("cls");
  }
  drawPic(yourAns, -1, -1, hintLine, hintRaw);
  cout << "正解!クリア!" << endl;


}
