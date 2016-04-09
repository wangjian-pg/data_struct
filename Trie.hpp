//for the information about trie and its usage
//reference to wiki at:https://en.wikipedia.org/wiki/Trie

#include <string>
#include <memory>
#include <map>
#include <iostream>

class   TrieNode {
    public:
        std::map<char,std::shared_ptr<TrieNode>>    nodes;
        bool                                        valid;
        TrieNode():valid(false){};
};



class Trie {
    typedef std::shared_ptr<TrieNode>   NodePtr;

    private:
        std::shared_ptr<TrieNode>    root;
        bool    _searchReg(const std::string&, int i, NodePtr)  const;

    public:

        Trie();
        void    insert(const std::string&);
        bool    search(const std::string&)  const;
        bool    startWith(const std::string&)   const;
        bool    searchReg(const std::string&)  const;

};
Trie::Trie():root(new TrieNode()) {
}


void    Trie::insert(const std::string& word)
{
        std::shared_ptr<TrieNode>   curNode =   root;
        for(int i = 0; i < word.size(); ++i){
            auto    ret =   curNode->nodes.insert(std::make_pair(word[i], std::shared_ptr<TrieNode>(new TrieNode())));
            curNode =   ret.first->second;
        }
        curNode->valid  =   true;
}


bool    Trie::search(const std::string& word)   const
{
        std::shared_ptr<TrieNode>   curNode =   root;
        for(int i = 0; i < word.size(); ++i){
            auto    iter    =   curNode->nodes.find(word[i]);
            if(iter == curNode->nodes.end()){
                return  false;
            }else{
                curNode =   iter->second;
            }
        }
        if(curNode&&curNode->valid) return  true;
        else{
             return false;
        }

}

//only support '.' witch can represent any one character
bool    Trie::searchReg(const std::string& word)    const
{
    return  _searchReg(word, 0, root);

}

bool    Trie::_searchReg(const std::string& word, int i, Trie::NodePtr trieNode)    const
{
     if(!trieNode || i > word.size())  return  false;
     if(trieNode&&i==word.size()&&trieNode->valid)  return  true;
    if(word[i]=='.'){
         for(const auto&   elem:   trieNode->nodes){
             if(_searchReg(word,i+1,elem.second)){
                 return true;
             }
        }
        return false;
    }else{
         auto   iter = trieNode->nodes.find(word[i]);
         if(iter==trieNode->nodes.end())
             return  false;
         else{
             return _searchReg(word, i+1, iter->second);
         }
    }
}

bool    Trie::startWith(const std::string& word)   const
{
        std::shared_ptr<TrieNode>   curNode =   root;
        for(int i = 0; i < word.size(); ++i){
            auto    iter    =   curNode->nodes.find(word[i]);
            if(iter == curNode->nodes.end()){
                return  false;
            }else{
                curNode =   iter->second;
            }
        }
        return  true;
}
