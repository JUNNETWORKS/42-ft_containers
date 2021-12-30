#ifndef RED_BLACK_TREE_H_
#define RED_BLACK_TREE_H_

namespace ft {

template <typename Key, typename Value>
struct RBTNode {
  enum Color { BLACK = 0, RED = 1 };

  Key key_;
  Value value_;
  RBTNode *parent_;
  RBTNode *left_;
  RBTNode *right_;
  Color color_;

  RBTNode(Key key, Value value = Value())
      : key_(key), value_(value), parent_(), left_(), right_(), color_(BLACK) {}
};

// Red Black Tree
//
// ノードを辿るルールは left < key <= right である.
//
// Red Black Tree の2色条件
// 1. 各ノードは赤か黒の色である
// 2. 根は黒である. (たまにこの条件は省かれる)
// 3. 派(NIL)は全て黒である. 葉は全て根と同じ色である.
// 4. 赤のノードは黒ノードを2つ子に持つ.
// 5. 任意のノードについて,
//    そのノードから子孫の葉までの未知に含まれる黒ノードの数は,
//    選んだ葉に依らず一定である.
//    (この条件は,
//    「根から葉までの道に含まれる黒いノードの数は、葉によらず一定である」
//    と言い換えることができる)
template <typename Key, typename Value>
class RedBlackTree {
 public:
  typedef RBTNode<Key, Value> node_type;

  // Constructor, Descructor

  RedBlackTree() : root_(nil_node_), nil_node_(&nil_node_object_) {}

  RedBlackTree(const RedBlackTree &other) {}

  RedBlackTree &operator=(const RedBlackTree &rhs) {}

  ~RedBlackTree() {}

  // Tree operations

  void Insert(Key key, Value value) {
    node_type *y = nil_node_;
    node_type *x = root_;
    while (x) {
      y = x;
      if (key < x->key_) {
        x = x->left_;
      } else {
        x = x->right_;
      }
    }
    node_type *new_node = node_type(key, value);
    new_node->parent_ = y;
    if (y == nil_node_) {
      root_ = new_node;
    } else if (new_node->key_ < y->key_) {
      y->left_ = new_node;
    } else {
      y->right_ = new_node;
    }
    new_node->left_ = nil_node_;
    new_node->right = nil_node_;
    new_node->color_ =
        node_type::RED;  // 新しいノードの色は最初は赤に設定される
    InsertFixup(new_node);
  }

  void Delete(Key key) {
    Delete(Search(key));
  }

  // ノードの削除
  //
  // q: 削除ノードの親ノード(左右は問わない)
  // z: 削除ノード
  // l: 削除ノードの左の子
  // r: 削除ノードの右の子
  // 1,2,3...: その他のノード
  //
  // パターン1: 削除ノードが左の子を持たない場合
  //      q                     q
  //      |                     |
  //      z          -->        r
  //       \                                                                 
  //        r
  //
  // パターン2: 削除ノードが右の子を持たない場合
  //      q                     q
  //      |                     |
  //      z          -->        l
  //     /
  //    l
  //
  // パターン3: 削除ノードが2つの子を持ち,
  //            削除ノードの右の子が次節点の場合
  //      q                       q
  //      |                       |
  //      z          -->          r
  //     / \                     / \                                                                                                    
  //    l   r                   l   1
  //         \ 
  //          1
  //
  // パターン4: 削除ノードが2つの子を持ち,
  //            削除ノードの右の部分木内にある場合
  //            (ただし削除ノードの右の子は次節点ではない)
  //      q                                                 q
  //      |                                                 |
  //      z          -- yの場所をxで置き換える -->          z
  //     / \           (次節点は左の子を持たない)          / \  
  //    l   r                                             l   r     2
  //       /                                                 /
  //      2                                                 3
  //       \                                                   
  //        3
  //
  //                                      q
  //                                      |
  //  -- zの部分をyに置き換える -->       2
  //                                     / \ 
  //                                    l   r
  //                                       /
  //                                      3
  void Delete(node_type *z) {
    // yがもともと置かれていた場所に移動する節点
    node_type *x;
    // 木からの削除あるいは木の中の移動が想定される節点.
    // zの子が1つの場合は削除対象, zの子が2つの場合は移動対象.
    node_type *y = z;
    // 節点yを再彩色する可能性があるので元の色を保持する.
    // yの代入直後のyの色を覚えておく.
    node_type::Color y_original_color = y->color;

    if (z->left_ == nil_node_) {
      x = z->right_;
      DeleteTransplant(z, z->right_);
    } else if (z->right_ == nil_node_) {
      x = z->left_;
      DeleteTransplant(z, z->left);
    } else {
      // 削除ノードが2つ子を持つ場合
      y = TreeMinimum(z->right_);
      y_original_color = y->color;
      x = y->right_;
      if (y->parent_ == z) {
        // 削除ノードの右の子が次節点の場合
        x->parent_ = y;
      } else {
        DeleteTransplant(y, y->right_);
        y->right_ = z->right_;
        y->right_->parent = y;
      }
      DeleteTransplant(y, y->right_);
      y->left = z->left_;
      y->left_->parent_ = y;
      y->color = z->color;
    }

    if (y_original_color == node_type::BLACK) {
      // yが黒ならば, Deleteの操作によって2色条件が崩れた可能性がある.
      // x(もともとyがあった場所)から上に2色条件を違反していないか見ていく
      //
      // yが赤ならば以下の3つの理由によってyの削除あるいは移動は2色条件を維持する.
      // 1. 木の黒高さは変化しない.
      // 2. 2つの赤節点が連続することは無い.
      //    なぜならyはzの色を継承した上でzの場所に移されるので,
      //    yの木の新しい場所で2つの赤節点が連続することはない.
      // 3. yが赤ならば, yが根であるはずはなく, 根は依然として黒である.
      //
      // yが黒ならば以下の3つの問題が発生する可能性がある.
      // 1. 元々はyが根であって, yの赤の子が新しい根になった時,
      //    2色条件その2(根は黒である)に違反する.
      // 2. xとx.parentが共に赤の時,
      //    2色条件その4(赤ノードは黒2つを子に持つ)に違反する.
      // 3. 木の中をyが移動すると,
      //    元々yを含んでいた単純道の黒節点数が1減少する可能性がある.
      //    これは2色条件その5(任意のノードは子孫の葉までに含まれる黒節点数は一定である)
      //    に違反する.
      DeleteFixup(x);
    }
  }

  node_type *Search(const Key &key) {
    node_type *current = root_;
    while (current && current->key_ != key) {
      if (current->key_ < key) {
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return current;
  }

 private:
  enum Direction { LEFT = 0, RIGHT = 1 };

  // Fix tree

  // RotateLeft
  //
  // xを11とし, xを中心に左回転すると以下のようになる. この場合yは18となる.
  //
  //    root                               root
  //     |                                  |
  //     7                                  7
  //   /   \                              /   \ 
  //  4     11                           4     18
  //       /  \     -- RotateLeft -->         /  \   
  //      9   18                             11   19
  //         /  \                           /  \ 
  //        14  19                         9   14
  void RotateLeft(node_type *x) {
    node_type *y = x->right_;  // y を x の右の子とする.
    x->right_ = y->left_;  // y の左部分木を x の右部分木にする.
    if (y->left_ != nil_node_) {
      // yの左の子の親が左回転後のxになるようにする
      y->left_->parent_ = x;
    }
    y->parent_ = x->parent_;  // xの親をyにする
    // xの親の左右どちらにyを付けるか
    if (x->parent_ == nil_node_) {
      // xが根だった場合
      root_ = y;
    } else if (x == x->parent_->left_) {
      x->parent_->left_ = y;
    } else {
      x->parent_->right_ = y;
    }
    // xをyの左の子とする.
    y->left_ = x;
    x->parent_ = y;
  }

  // RotateRight
  //
  // xを6とし, xを中心に右回転すると以下のようになる. この場合yは4となる.
  //
  //       root                                root
  //        |                                   |
  //        10                                  10
  //      /   \                               /   \   
  //     6     12                            4     12
  //    /  \         -- RotateRight -->     /  \      
  //   4    8                              2    6
  //  /  \                                     / \     
  // 2    5                                   5   8
  void RotateRight(node_type *x) {
    node_type *y = x->left_;  // y を x の左の子とする.
    x->left_ = y->right_;  // y の右部分木を x の左部分木にする.
    if (y->right_ != nil_node_) {
      // yの右の子の親が右回転後のxになるようにする
      y->right_->parent_ = x;
    }
    y->parent_ = x->parent_;  // xの親をyにする
    // xの親の左右どちらにyを付けるか
    if (x->parent_ == nil_node_) {
      // xが根だった場合
      root_ = y;
    } else if (x == x->parent_->right_) {
      x->parent_->right_ = y;
    } else {
      x->parent_->left_ = y;
    }
    // xをyの右の子とする.
    y->right_ = x;
    x->parent_ = y;
  }

  // 挿入時にRBTreeの2色条件を維持するための関数
  //
  // 修正パターンは3通り * 左右2通り で合計6通りある.
  // 以下に左の修正パターン3つを示す. 右バージョンは左右逆にすれば良い.
  //
  // R: Red
  // B: Black
  //
  // g: grant parent
  // p: parent
  // u: uncle
  // n: new node
  //
  // 修正パターン1: 叔父ノードが赤色.
  //              g_B                                          g_R
  //             /   \                                        /   \       
  //           p_R    u_R     -- Change color g,p,u -->     p_B   u_B
  //           /                                            /
  //         n_R                                          n_R
  //
  //                 MEMO: gを赤に彩色することで, 2色条件の5番が保たれる
  //
  //         -- Recheck parents colors from g_R to root -->     ...
  //
  // 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の左の子
  //              g_B                                     p_R
  //             /   \                                   /   \       
  //           p_R    u_B     -- Rotate right g -->    n_R    g_B
  //           /                                                \ 
  //         n_R                                                u_B
  //
  //                                             p_B
  //                                            /   \       
  //         -- Change color p,g -->          n_R   g_R
  //                                                  \ 
  //                                                  u_B
  //
  // 修正パターン3: 叔父ノードが黒色 + 挿入するノードが親の右の子
  //              g_B                                       g_B
  //             /   \                                     /   \    
  //           p_R    u_B     -- Rotate Left p -->       n_R    u_B
  //             \                                       /
  //             n_R                                   p_R
  //
  //                                         n_B
  //                                        /   \                                   
  //         -- Apply pattern2 -->        p_R    g_R
  //                                              \                                                         
  //                                               u_B
  void InsertFixup(node_type *new_node) {
    // 新しいノードは赤色であり, 赤ノードは赤ノードを子に持つことは出来ない.
    while (new_node->parent_->color_ == node_type::RED) {
      if (new_node->parent_ == new_node->parent_->parent_->left_) {
        // 新しいノードの親は祖父の左の子
        // 叔父ノード
        node_type *uncle_node = new_node->parent_->parent_->right_;
        if (uncle_node->color_ == node_type::RED) {
          // 修正パターン1: 親と叔父ノードが赤色
          // 親と叔父ノードを黒にし, 祖父を赤にする.
          new_node->parent_->color_ = node_type::BLACK;
          uncle_node->color_ = node_type::BLACK;
          new_node->parent_->parent_->color_ = node_type::RED;
          new_node = new_node->parent_->parent_;
        } else {
          // 修正パターン3 の後半部分の処理は パターン2 と同じなのでまとめられる

          if (new_node == new_node->parent_->right_) {
            // 修正パターン3: 叔父ノードが黒色 + 挿入するノードが親の右の子
            new_node = new_node->parent_;
            RotateLeft(new_node);
          }
          // 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の左の子
          new_node->parent_->color_ = node_type::BLACK;
          new_node->parent_->parent_->color_ = node_type::RED;
          RotateRight(new_node->parent_->parent_);
        }
      } else {
        // 新しいノードの親は祖父の右の子
        // 叔父ノード
        node_type *uncle_node = new_node->parent_->parent_->left_;
        if (uncle_node->color_ == node_type::RED) {
          // 修正パターン1: 親と叔父ノードが赤色
          // 親と叔父ノードを黒にし, 祖父を赤にする.
          new_node->parent_->color_ = node_type::BLACK;
          uncle_node->color_ = node_type::BLACK;
          new_node->parent_->parent_->color_ = node_type::RED;
          new_node = new_node->parent_->parent_;
        } else {
          // 修正パターン3 の後半部分の処理は パターン2 と同じなのでまとめられる

          if (new_node == new_node->parent_->left_) {
            // 修正パターン3: 叔父ノードが黒色 + 挿入するノードが親の左の子
            new_node = new_node->parent_;
            RotateRight(new_node);
          }
          // 修正パターン2: 叔父ノードが黒色 + 挿入するノードが親の右の子
          new_node->parent_->color_ = node_type::BLACK;
          new_node->parent_->parent_->color_ = node_type::RED;
          RotateLeft(new_node->parent_->parent_);
        }
      }
    }
  }

  // ある節点の子であるuを根とする部分木を別の節点の子のvを根とする部分木に置き換える.
  // v.left_, v.right_ に対しては変更を行わないので注意.
  // ノードを削除する際に使う
  void DeleteTransplant(node_type *u, node_type *v) {
    if (u->parent_ == nil_node_) {
      // uが根のとき
      root_ = v;
    } else if (u == u->parent_->left_) {
      u->parent_->left_ = v;
    } else {
      u->parent_->right_ = v;
    }
    v->parent_ = u->parent_;
  }

  // ノード削除によって2色条件が違反が発生したか調べ, 発生していたら修正する.
  //
  // R: 赤
  // B: 黒
  // p: 親
  // x: 今見ているノード
  // b: 兄弟
  // l: 兄弟の左の子
  // r: 兄弟の右の子
  //
  //
  // 修正するパターンは兄弟の色と兄弟の子の色によって以下の4つに分けられる.
  // (明示的に色指定されていないノードは何色でも良い(例えば親ノードとか))
  // 基本的にxが親の左の子という想定で書いてる.
  // 右バージョンは左右逆にすれば良い.
  //
  // 修正パターン1: 兄弟が赤
  //       p_B                                             p_R
  //      /   \                                           /   \                                                                                                                     
  //    x_B   b_R        -- 親と兄弟の色を変える -->    x_B   b_B
  //         /   \                                           /   \                                                                                                                 
  //       l_B   r_B                                       l_B   r_B
  //
  //                              b_B
  //                             /   \          
  //    -- 親を左回転 -->      p_R   r_B
  //                          /   \ 
  //                        x_B   l_B
  //
  //    -- 修正パターン2,3,4で修正する -->
  //
  // 修正パターン2: 兄弟が黒 + 兄弟の子が両方黒
  //       p_B                                   p_B
  //      /   \                                 /   \                 
  //    x_B   b_B      -- 兄弟を赤にする -->  x_B   b_R
  //         /   \                                 /   \        
  //       l_B   r_B                             l_B   r_B
  //
  //    -- xを親に設定し, ループを継続 -->
  //
  // 修正パターン3: 兄弟が黒 + 兄弟の左の子が赤, 右の子が黒
  //       p_B                                                        p_B
  //      /   \                                                      /   \                                                              
  //    x_B   b_B      -- 兄弟と兄弟の左の子で色を交換する -->     x_B   b_R
  //         /   \                                                      /   \                                                                             
  //       l_R   r_B                                                  l_B   r_B
  //
  //       p_B                                 p_B
  //      /   \                               /   \                                      
  //    x_B   b_R      -- 兄弟を右回転 -->  x_B   l_B
  //         /   \                                  \                                                     
  //       l_B   r_B                                b_R
  //                                                  \                    
  //                                                  r_B
  //
  //    -- 修正パターン4で修正する -->
  //
  // 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤 (左の子は赤でも黒でもいい)
  //       p_R                                                       p_B
  //      /   \                                                     /   \                                          
  //    x_B   b_B     -- 親と兄弟の色を交換 & 兄弟の右を黒に -->  x_B   b_R
  //         /   \                                                     /   \                                     
  //       l_B   r_R                                                 l_B   r_B
  //
  //                          b_R
  //                         /   \                                          
  //    -- 親を左回転 -->  p_B   r_B
  //                      /   \   
  //                    x_B   l_B
  //
  void DeleteFixup(node_type *x) {
    while (x != root_ && x->color_ == node_type::BLACK) {
      if (x == x->parent_->left_) {
        // 兄弟ノード
        node_type *w = x->parent_->right_;
        if (w->color_ == node_type::RED) {
          // 修正パターン1: 兄弟が赤
          w->color_ = node_type::BLACK;
          x->parent_->color_ = node_type::RED;
          RotateLeft(x->parent_);
          w = x->parent_->right_;
        }
        if (w->left_->color_ == node_type::BLACK &&
            w->right_->color_ == node_type::BLACK) {
          // 修正パターン2: 兄弟が黒 + 兄弟の子が両方黒
          w->color_ = node_type::RED;
          x = x->parent_;
        } else {
          if (w->right_->color_ == node_type::BLACK) {
            // 修正パターン3: 兄弟が黒 + 兄弟の左の子が赤, 右の子が黒
            w->left_->color_ = node_type::BLACK;
            w->color_ = node_type::RED;
            RotateRight(w);
            w = x->parent_->right_;
          }
          // 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤
          w->color_ = x->parent_->color_;
          x->parent_->color_ = node_type::BLACK;
          w->right_->color_ = node_type::BLACK;
          RotateLeft(x->parent_);
          x = root_;
        }
      } else {
        // 兄弟ノード
        node_type *w = x->parent_->left_;
        if (w->color_ == node_type::RED) {
          // 修正パターン1: 兄弟が赤
          w->color_ = node_type::BLACK;
          x->parent_->color_ = node_type::RED;
          RotateLeft(x->parent_);
          w = x->parent_->left_;
        }
        if (w->right_->color_ == node_type::BLACK &&
            w->left_->color_ == node_type::BLACK) {
          // 修正パターン2: 兄弟が黒 + 兄弟の子が両方黒
          w->color_ = node_type::RED;
          x = x->parent_;
        } else {
          if (w->left_->color_ == node_type::BLACK) {
            // 修正パターン3: 兄弟が黒 + 兄弟の左の子が赤, 右の子が黒
            w->right_->color_ = node_type::BLACK;
            w->color_ = node_type::RED;
            RotateRight(w);
            w = x->parent_->left_;
          }
          // 修正パターン4: 兄弟が黒 + 兄弟の右の子が赤
          w->color_ = x->parent_->color_;
          x->parent_->color_ = node_type::BLACK;
          w->left_->color_ = node_type::BLACK;
          RotateLeft(x->parent_);
          x = root_;
        }
      }
    }
    x->color_ = node_type::BLACK;
  }

  // Members
  RBTNode<Key, Value> *root_;
  // NIL node. NIL node isn't stored in heap area.
  RBTNode<Key, Value> *nil_node_;
  RBTNode<Key, Value> nil_node_object_;
};

template <class Key, class Value>
RBTNode<Key, Value> *TreeMinimum(const RBTNode<Key, Value> *root) {
  typedef RBTNode<Key, Value> node_type;

  if (!root) {
    return NULL;
  }

  node_type *current = root;
  while (current->left_ != NULL) {
    current = current->left_;
  }
  return current;
}

template <class Key, class Value>
RBTNode<Key, Value> *TreeMaximum(const RBTNode<Key, Value> *root) {
  typedef RBTNode<Key, Value> node_type;

  if (!root) {
    return NULL;
  }

  node_type *current = root;
  while (current->right_ != NULL) {
    current = current->right_;
  }
  return current;
}

// 中間順木巡回の順序での次の節点のポインタを返す
template <class Key, class Value>
RBTNode<Key, Value> *TreeSuccessor(const RBTNode<Key, Value> *current);

}  // namespace ft

#endif /* RED_BLACK_TREE_H_ */