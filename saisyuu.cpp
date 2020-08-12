#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <time.h>
#include <math.h>
using namespace std;

/*�}�X�ڂ̍\���̂��`*/
struct Grid {
  //bool�^�ŐF������,true�Ȃ�L�F,false�Ȃ疳�F
  bool yourColor = false; //���݂̃}�X�ڂ̐F
  bool collectColor = false; //�����̐F
  void changeYourColor(); //�}�X�ڂ̐F��ύX
  bool checkYourAns(); // ���̃}�X�ڂɂ��Đ������m�F
};

void Grid::changeYourColor() {
  yourColor = !yourColor; //���̃}�X�ڂ̐F��ύX
}

bool Grid::checkYourAns() {
  /*�v���C���̐F�Ɠ����������Ă��邩�m�F*/
  if (yourColor == collectColor) return true;
  return false;
}


/*���𐶐�*/
vector<vector<Grid>> makeAns(int hight, int width) {
  vector<vector<Grid>> ans(hight, vector<Grid>(width)); //Grid�^��vector���`
  int random; //�����𐶐�

  /*�����Ŋe�}�X�ڂ̐F(����)������*/
  for (int y = 0; y < hight; y++) {
    static clock_t timPre, timCur = clock();
    srand((unsigned int)timCur); /*���Ԃŏ�����*/
    for (auto y = ans.begin(); y != ans.end(); y++) {
      for (auto x = (*y).begin(); x != (*y).end(); x++) {
        random = rand() % 2; //0�܂���1�̗����𐶐�
        if (random == 1) {
          (*x).collectColor = true; //0�Ȃ疳�F�ɐݒ�
        }else {
          (*x).collectColor = false; //1�Ȃ�L�F(��)�ɐݒ�
        }
      }
    }
    /*�����p�^�[���̗����𐶐���h��_�����Ԋu���J����*/
    timPre = timCur;
    while ((unsigned int)timPre == (unsigned int)timCur) {
      timCur = clock();
    }
  }
  return ans;
}


/*�s�q���g���̎擾*/
vector<vector<int>> getHintLine(vector<vector<Grid>> ans, int hight, int width){
  vector<vector<int>> hintLine(hight, vector<int>()); //�s�̃q���g���i�[���邽�߂�vector�𐶐�
  for (int i = 0; i < hight; i++) {
    static int count = 0;
    for (int j = 0; j < width; j++) {
      if (ans.at(i).at(j).collectColor) {
        count++;
      }
      if (!ans.at(i).at(j).collectColor && count != 0) {
        hintLine[i].push_back(count); //�����}�X�ڂ��A������񐔂���
        count = 0;
      }
    }
    if (count != 0) {
      hintLine[i].push_back(count);
    }
    if (hintLine.at(i).empty()) {
      hintLine[i].push_back(0); //���̗�ɍ����}�X�ڂ�1���Ȃ��ꍇ��0����
    }
    count = 0;
  }
  return hintLine;
}


/*��q���g����̎擾*/
vector<vector<int>> getHintRaw(vector<vector<Grid>> ans, int hight, int width){
  vector<vector<int>> hintRaw(width, vector<int>());
  for (int x = 0; x < width; x++) {
    static int count = 0;
    for (int y = 0; y < hight; y++) {
      if (ans.at(y).at(x).collectColor) {
        count++;
      }
      if (!ans.at(y).at(x).collectColor && count != 0) {
        hintRaw[x].push_back(count); //�����}�X�ڂ̘A���񐔂���
        count = 0;
      }
    }
    if (count != 0) {
      hintRaw[x].push_back(count);
    }
    if (hintRaw.at(x).empty()) { //���̗�ɍ����}�X�ڂ�1���Ȃ��ꍇ��0����
      hintRaw[x].push_back(0);
    }
    count = 0;
  }
  return hintRaw;
}



/*���ƕʖ��̉��������ɂȂ邩�m�F*/
bool isSameAns(vector<vector<Grid>> anoSol, vector<vector<int>>hintLOrg, vector<vector<int>>hintROrg) {
  int hight, width;
  hight = end(anoSol) - begin(anoSol);
  width = end(anoSol.at(0)) - begin(anoSol.at(0));
  vector<vector<int>> hintL, hintR;
  hintL = getHintLine(anoSol, hight, width); //�ʉ����̍s�q���g���𒲍�
  hintR = getHintRaw(anoSol, hight, width); //�ʉ��̗�q���g���𒲍�
  if (hintL == hintLOrg && hintR == hintROrg) {
    return true;
  }
  return false;
}


/*�g�ݍ��킹�v�Z*/
void combination(const vector<int>& color, vector<int>& result, vector<int>& box, unsigned n) {
  if (result.size() == n) { //�w��I�𐔂܂ŒT�����������f
    for (auto& i : result) {
      box.push_back(i); //�g�ݍ��킹�����L�^
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



/*�S�v�f��false���琬��2����vector�̐���*/
vector<vector<Grid>> resetV(int hight, int width) {
  vector<vector<Grid>> v(hight, vector<Grid>(width));
  for (auto i = v.begin(); i != v.end(); i++) {
    for (auto j = (*i).begin(); j != (*i).end(); j++) {
        (*j).collectColor = false;
    }
  }
  return v;
}






/*�쐬�������̈�Ӑ����m�F*/
bool checkAnoSol(int hight, int width, vector<vector<int>>hintLOrg, vector<vector<int>>hintROrg) {
  vector<vector<Grid>> anoSol; //�ʉ�T���p
  vector<int> choice, result, comb; //�}�X�ڂ̐����̐��l�i�[�p,�g�ݍ��킹�̈ꎞ�ێ��p,�g�ݍ��킹�S�L�^�p
  int solutions = 0, numcolor = 0, combElements = 0, numElements; //���̌�,�����}�X�ڂ̍��v,�g�ݍ��킹�i�[�p,�}�X�ڂ̍��v��
  numElements = hight * width; //�}�X�ڂ̍��v�����v�Z
  /*�͔͉𓚂̍����}�X�ڂ̍��v���𒲍�*/
  for (auto i = hintLOrg.begin(); i != hintLOrg.end(); i++) {
    for (auto j = (*i).begin(); j != (*i).end(); j++) {
        numcolor += *j;
    }
  }
  /*�}�X�ڂ̍��v���Ɠ���������vector�𐶐�*/
  for (int i = 0; i < numElements; i++) {
    choice.push_back(i);
  }
  combination(choice, result, comb, numcolor); //�F��h��}�X�ڂ̑g�����𒲍�
  for (auto i = comb.begin(); i != comb.end(); i++) {
    combElements++;
  }
  /*�ʉ�T��*/
  for (int i = 0; i < combElements / numcolor; i++) {
    anoSol = resetV(hight, width);
    for (int j = 0; j < numcolor; j++) { //numcolor�͗L�F�}�X�̌�
      anoSol.at(comb[i * numcolor + j] / width).at(comb[i * numcolor + j] % width).collectColor = true; //�w�肵���s,��̐F������
    }
    if (isSameAns(anoSol, hintLOrg, hintROrg)) solutions++; //���������ɂȂ邩�m�F
    if(1 < solutions) return false; //���������o����A�E�g
  }
  if (solutions == 1) return true; //����1�Ȃ�OK
  else return false;
}




/*1�s���̃q���g���\��*/
void printHintL(int y, vector<vector<int>> hintLine) {
  int sizeVecL;
  cout << (" ");
  if (!hintLine.at(y).empty()) {
    sizeVecL = end(hintLine.at(y)) - begin(hintLine.at(y));
    for (int i = 0; i < sizeVecL; i++) {
      char num[3] = ("�O"); //�S�p�����\���p
      num[1] = num[1] + hintLine.at(y).at(i); //�S�p�ɕϊ�
      cout << num;
    }
  }
}


/*�S���񕪂̃q���g��\��*/
void printHintR(vector<vector<int>> hintRaw) {
  int sizeVecL, maxSizeVecL = 0, sizeVecR; //��q���gvec�̍s��,�ő�s��,��
  sizeVecR = end(hintRaw) - begin(hintRaw);
  /*��q���gvecctor�̍ő�s���𒲍�*/
  for (int i = 0; i < sizeVecR; i++) {
    sizeVecL = end(hintRaw.at(i)) - begin(hintRaw.at(i));
    if (maxSizeVecL < sizeVecL) {
      maxSizeVecL = sizeVecL;
    }
  }
  /*��q���g��S�ĕ\��*/
  for (int i = 0; i < maxSizeVecL; i++) {
    cout << ("�@");
    for (int j = 0; j < sizeVecR; j++) {
      sizeVecL = end(hintRaw.at(j)) - begin(hintRaw.at(j));
      if (i < sizeVecL) {
        char num[3] = ("�O"); //�S�p�ϊ��p
        num[1] = num[1] + hintRaw.at(j).at(i); //�S�p�����Ƃ��ďo��
        cout << num;
      } else {
        cout << ("�@");
      }
    }
    cout << endl;
  }
}


/*�v���C���[�̉𓚂Ɩ͔͉𓚂���v���邩�m�F*/
bool checkYourAns(vector<vector<Grid>> grids) {
  for (auto y = grids.begin(); y != grids.end(); y++) {
    for (auto x = (*y).begin(); x != (*y).end(); x++) {
      if (!(*x).checkYourAns()) return false;
    }
  }
  return true;
}



/*�`�掞�̃}�[�J�[�̑I��*/
void selectPatern(int y, int posY, int x, int posX, bool yourColor) {
  if ((x == posX) && (y == posY)) {// �J�[�\���̈ʒu��\��
    if (yourColor) {
      cout << "��";
    } else {
      cout << "��";
    }
  }else { //�}�X�ڕ`��
    if (yourColor) {
      cout << "��"; //�����}�X�ڂ�\��
    } else {
      cout << "��"; //�����}�X�ڂ�\��
    }
  }
}


/*�`�揈��*/
void drawPic(vector<vector<Grid>>& grids, int posY, int posX, vector<vector<int>> hintLine, vector<vector<int>> hintRaw) {
  cout << endl;
  for (size_t y = 0; y < grids.size(); y++) {
    cout << ("�@");
    for (size_t x = 0; x < grids.at(y).size(); x++) {
      selectPatern(y, posY, x, posX, grids.at(y).at(x).yourColor); //�}�X�ڂ̖͗l��\��
    }
    printHintL(int(y), hintLine); //�s�q���g�̕\��
    cout << endl;
  }
  printHintR(hintRaw); //�Ō�ɗ�q���g�̕\��
}



/*���͏���*/
void input(vector<vector<Grid>>& grids, int &posY, int &posX, int hight, int width) {
  switch (_getch()) { //�L�[�{�[�h���͂̎�t
    case 'p': //p�����͂��ꂽ�炻�̃}�X�ڂ̐F��ς���
      grids.at(posY).at(posX).changeYourColor();
      break;
    case 'u': //u�����͂��ꂽ��J�[�\������Ɉړ�
      posY--;
      if (posY < 0) posY = 0; //�ړ�����
      break;
    case 'd': //d�����͂��ꂽ��J�[�\�������Ɉړ�
      posY++;
      if (hight <= posY) posY --; //�ړ�����
      break;
    case 'r':
      posX++; //r�����͂��ꂽ��J�[�\�����E�Ɉړ�
      if (width <= posX) posX --; //�ړ�����
      break;
    case 'l':
      posX--; //l�����͂��ꂽ��J�[�\�������Ɉړ�
      if (posX < 0) posX = 0; //�ړ�����
      break;
    default :
      break;
  }
}



int main() {
  const int hight = 4, width = 4; //�}�X�ڂ̃T�C�Y_�K��1���ɐݒ�
  vector<vector<Grid>> grids(hight, vector<Grid>(width)); //�v���C���[�̉𓚏����i�[
  vector<vector<int>> hintLine; //�s�q���g���
  vector<vector<int>> hintRaw; //��q���g���

  /*��萶������*/
  cout << ("���쐬��");
  do {
    grids.clear(); //�ēx��萶������vector�̒��g������
    hintLine.clear(); //�ēx��萶������vector�̒��g������
    hintRaw.clear(); //�ēx��萶������vector�̒��g������
    grids = makeAns(hight, width); //��萶��
    hintLine = getHintLine(grids, hight, width);
    hintRaw = getHintRaw(grids, hight, width);
  } while(!checkAnoSol(hight, width, hintLine, hintRaw)); //��₪�K�����m�F
  system("cls");

  /*�`�揈��*/
  while (!checkYourAns(grids)) {
    static int posX = 0, posY = 0; //�J�[�\���̍��W��`
    drawPic(grids, posY, posX, hintLine, hintRaw); //�`�揈��
    cout << endl << endl;
    input(grids, posY, posX, hight, width); //���͏���
    system("cls"); //��ʏ���
  }
  drawPic(grids, -1, -1, hintLine, hintRaw);
  cout << "����!�N���A!" << endl;


}
