#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;

map<char, pair<double,double> > m; //символ соответствует своему отрезку на отрезке [0,1]
map<char,double> sym_freq; //вероятность символа



int main(int argc, char* argv[])
{

    ifstream f("input.txt", ios::out | ios::binary);
    ofstream g("encode.txt", ios::out | ios::binary);


    /*
     * считывая символы, составим алфавит
     * Каждому символу сопоставляется количество встреч в строке
     */
    char c;
    c = f.get();
    int count = 0;

    while (!f.eof())
    {
        m[c].first++;
        c=f.get();
        count ++;
    }



    //определим верхнюю и нижнюю границы
    double high = 1.0;
    double low = 0.0;

    //чтобы пропорционально разбить отрезок [0,1], для каждого символа найдём его отрезок
    /*
     * в pair - содержатся два числа double: это первая и вторая границы соответственно, которые нужны для разбиения
     * проходясь по всем возможным символам, получим его отрезок
     */
    for (map<char, pair<double,double> >::iterator itr = m.begin(); itr != m.end(); ++itr)
    {

        itr->second.first/=count; //вероятность
        sym_freq[itr->first] = 	itr->second.first; //связываю символ и его вероятность

        itr->second.first = low;
        itr->second.second = itr->second.first + sym_freq[itr->first];
        low = itr->second.second;

    }

    //в файл g получим таблицу вероятнотей
    for (map<char, double >::iterator itr = sym_freq.begin(); itr != sym_freq.end(); ++itr)
        g<<itr->first<<" "<<itr->second<<" ";

    g<<"|"<<count<<" "; //отметим конец записи символов


    //encoding


    low = 0.0;
    f.clear(); f.seekg(0);


    c = f.get();
    while (!f.eof())
    {
        //считаю новые отрезки для символа
        double range = high - low;
        high = low + range * m[c].second; //m[c].second - правая граница
        low = low + range * m[c].first; // m[c].first - левая
        ////cout<<low<<" "<<high<<endl;

        //новый символ
        c = f.get();

    }
    //g<<low+(high-low)/2<< " ";

    f.close();

    double d = 0; //искомое число
    int deg = 1; //степень
    double temp=1;
    /*
     * ищем ряд слогаемых вида 1/(2^n)
     * т.е. если слагаемое используется (в двоичной системе счисления)
     * то добавляем 1, если нет, 0
     * таким образом получим искомое число в двоичной системе счисления
     */
    vector <bool> v;
    cout<<low<<" "<<high<<endl;
    while(d<low || d>high){
        for(int i=0;i<deg;i++)
            temp*=2;
        d+=(1/temp);
        if(d>high) {
            d-=(1/temp);
            v.push_back(0);

        } else v.push_back(1);
        deg++;
        temp=1;

    }

    //вывод
    for(int i=0;i<v.size();i++)
        cout<<v[i];
    char buf;
    count = 0;

    for (int n = 0; n < v.size(); n++)
    {
        buf = buf | v[n] << (7 - count);
        count++;
        if (count == 8) { count = 0;   g << buf; buf = 0; }
    }
    if(buf) g<<buf;

    g.close();



    return 0;
}