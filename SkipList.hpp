#include    <algorithm>
#include    <iostream>
#include    <random>
#include    <cassert>
#include    <stdlib.h>
#include    <boost/smart_ptr/scoped_array.hpp>
#include    <boost/smart_ptr/shared_array.hpp>
#include    <memory>


//TODO
//1.finish the iterator interface
class   SkipList{
private:
    size_t          maxLevel;
    size_t          level;
    size_t          _size;
//    const   double  p   =   0.5;
    //static  const   int MAX_VAL =   0x7FFFFFFF;

    class   iterator{

    };

    struct  ListNode{
        int             val;
        size_t          level;
        boost::scoped_array<size_t>                     wides;
        boost::scoped_array<std::shared_ptr<ListNode>>  forwardNodes;

        ListNode(int _val, size_t _level):val(_val),level(_level),
            forwardNodes(new std::shared_ptr<ListNode>[_level]()),
            wides(new  size_t[_level]){}

        ListNode():val(0),level(0){}
    };

    std::shared_ptr<ListNode>       head;

    size_t  CalcNoteLevel(){
        static  int i   =   0;
        std::srand(time(nullptr)+i++);
        return  (std::rand()%maxLevel+1);
    }

public:
    SkipList(size_t _maxLevel):level(1),maxLevel(_maxLevel),_size(0),head(new ListNode(0,maxLevel))
    {
        for(int i = 0; i < maxLevel;++i){
            head->wides[i]           =   0;
        }
    }


    ~SkipList(){
    }

    int Insert(int  _val){
        std::cout<<"Insert:"<<_val<<std::endl;
        boost::scoped_array<std::shared_ptr<ListNode>>      updateNodes(new std::shared_ptr<ListNode>[maxLevel]());
        boost::scoped_array<int>            wides(new int[maxLevel]());
        std::shared_ptr<ListNode>   curNode =   head;
        int      curLevel    =  this->maxLevel-1;
        do{
            if(curNode->forwardNodes[curLevel] && curNode->forwardNodes[curLevel]->val < _val){
                wides[curLevel]    +=   curNode->wides[curLevel];
                curNode =  curNode->forwardNodes[curLevel];
            }else{
                updateNodes[curLevel] =   curNode;
                --curLevel;
            }
        }while(curLevel>=0);

       // std::cout<<"wides:";
       // for(int i = 0; i < maxLevel;++i){
       //     std::cout<<wides[i]<<"  ";
       // }
       // std::cout<<std::endl;
        size_t  _NodeLevel  =   CalcNoteLevel();
       //std::cout<<"NodeLevel:"<<_NodeLevel<<std::endl;

        std::shared_ptr<ListNode>   _newNode(new ListNode(_val,_NodeLevel));

        int step    =   1;
        for(int i = 0; i < level && i < _NodeLevel ;++i){
            _newNode->wides[i]  =   updateNodes[i]->wides[i]  -   step  +   1;
            updateNodes[i]->wides[i]    =   step;
            step    +=  wides[i];
            _newNode->forwardNodes[i]   =   updateNodes[i]->forwardNodes[i];
            updateNodes[i]->forwardNodes[i] =   _newNode;
        }

        if(_NodeLevel > level){
            for(int i = level; i < _NodeLevel;++i){
                _newNode->forwardNodes[i]   =   head->forwardNodes[i];
                _newNode->wides[i]  =   head->wides[i]-step+1;
                head->wides[i]      =   step;
                head->forwardNodes[i] =   _newNode;
            }
            level    =   _NodeLevel;
        }
       // else{
       //     for(int i = _NodeLevel; i < level;++i){
       //         updateNodes[i]->wides[i]+=1;
       //     }
       // }
        for(int i = _NodeLevel;i < maxLevel;++i)
            updateNodes[i]->wides[i]  +=  1;

        //update the size of this skiplist
        ++this->_size;

        return  step;
    }

    bool    RemoveByValue(int   _val){
        //std::cout<<"remove value:"<<_val<<std::endl;
        boost::scoped_array<std::shared_ptr<ListNode>>  updateNodes(new std::shared_ptr<ListNode>[maxLevel]());
        std::shared_ptr<ListNode>   curNode =   this->head; int curLevel    =   maxLevel-1;
        do{
            if(curNode->forwardNodes[curLevel]&&curNode->forwardNodes[curLevel]->val < _val)
                curNode =   curNode->forwardNodes[curLevel];
            else{
                updateNodes[curLevel]   =   curNode;
                --curLevel;
            }
        }while(curLevel>=0);
        if(curNode->forwardNodes[0]->val !=  _val){
            //can't find the _val in this skip list
            return  false;
        }else{
            curNode =   curNode->forwardNodes[0];
            for(curLevel = 0; curLevel < curNode->level; ++curLevel){
                updateNodes[curLevel]->forwardNodes[curLevel]   =   curNode->forwardNodes[curLevel];
                updateNodes[curLevel]->wides[curLevel]                 +=  curNode->wides[curLevel] -   1;
            }
            for(;curLevel < maxLevel;++curLevel){
                --updateNodes[curLevel]->wides[curLevel];
            }

            --this->_size;
            return  true;
        }
    }

    bool    RemoveByRank(int _rank){
        int _val = this->GetValueByRank(_rank);
         return this->RemoveByValue(_val);
    }

    int     GetValueByRank(int  rank){
        if(rank < 1)  rank    =   1;
        std::shared_ptr<ListNode>   curNode =   head;   int curLevel    =   this->level - 1;
        int _rank   =   0;
        do{
            if(!curNode->forwardNodes[curLevel] || _rank + curNode->wides[curLevel] > rank){
                --curLevel;
            }else{
                if(curNode->wides[curLevel] + _rank < rank){
                    _rank   +=  curNode->wides[curLevel];
                    curNode =   curNode->forwardNodes[curLevel];
                }else{
                    return  curNode->forwardNodes[curLevel]->val;
                }
            }
        }while(curLevel>=0);
        //when it comes here, the rank is larger that the size of this skip list
        return  curNode->val;
    }

    void    PrintInsideState(){
        std::cout<<"PrintInsideState"<<std::endl;
        for(int i = this->maxLevel-1 ;i >= 0;--i){
            std::shared_ptr<ListNode>   _head   =   head->forwardNodes[i];
            int wide    =   head->wides[i];
            while(_head){
                std::cout<<"<-"<<wide<<"->  "<<_head->val;
                wide    =   _head->wides[i];
                _head   =   _head->forwardNodes[i];
            }
            std::cout<<std::endl;
        }
        std::cout<<"======================================================"<<std::endl;
    }

    int     GetRank(int _val){
         int    _rank   =   0;
         std::shared_ptr<ListNode>  curNode =   this->head;
         int    curLevel    =   this->level;
         //std::cout<<"curLevel:"<<curLevel<<"    steps:";
         do{
             if(curNode->forwardNodes[curLevel]&&curNode->forwardNodes[curLevel]->val<=_val){
                 if(curNode->forwardNodes[curLevel]->val==_val){
                    //std::cout<<curNode->wides[curLevel]<<" ";
                    //std::cout<<std::endl;
                    return    _rank+curNode->wides[curLevel];
                 }
                 //std::cout<<curNode->wides[curLevel]<<" ";
                 _rank  +=  curNode->wides[curLevel];
                 curNode    =   curNode->forwardNodes[curLevel];
             }else{
                 --curLevel;
             }
         }while(curLevel>=0);
         //std::cout<<std::endl;
         if(curNode->forwardNodes[0]&&curNode->forwardNodes[0]->val == _val){
              return    curNode->wides[0]    +   _rank;
         }else{
             return 0;
         }
    }

    size_t  size()  const   {   return  this->_size;}

};


