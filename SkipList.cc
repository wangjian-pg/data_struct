#include    "SkipList.hpp"
#include    <vector>
int main(){
    SkipList    _skipList(10);
    std::vector<int>    _vector;
    for(int i = 0 ; i < 50; ++i){
        std::srand(time(nullptr)+i);
        int _val    =   std::rand()%100;
        _vector.push_back(_val);
        _skipList.Insert(_val);
    }

    _skipList.PrintInsideState();

    for(int i = 0; i < 10; ++i){
        //std::srand(time(nullptr)+i);
        //int    index   =   std::rand()%_vector.size();
        //int    _val    =   _vector[index];
        //_vector.erase(_vector.begin()+index);
        //_skipList.RemoveByValue(_val);
        //std::cout<<"after remove val:"<<_val;
        _skipList.RemoveByRank(i);
    }



    std::cout<<"after remove:"<<std::endl;
    _skipList.PrintInsideState();
    for(int i = _skipList.size()+3;i > -4;--i){
        std::cout<<"the "<<i<<"'th elem:"<<_skipList.GetValueByRank(i)<<std::endl;
    }

//    std::sort(_vector.begin(),_vector.end());
//    for(const   auto&   elem:_vector){
//        int _rank   =   _skipList.GetRank(elem);
//        std::cout<<"val:"<<elem<<"  rank:";
//        std::cout<<_rank<<std::endl;
//    }

    return  0;
}
