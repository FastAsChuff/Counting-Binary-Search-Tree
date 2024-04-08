typedef struct CBSTnode {
  struct CBSTnode *parent; 
  struct CBSTnode *left; 
  struct CBSTnode *right; 
  int64_t val;
  uint32_t lt;
  uint32_t eq;
  uint32_t gt;
} CountingBSTnode_t;

void CountingBSTfree(struct CBSTnode **root) {
  struct CBSTnode *thisnode_ptr = *root;
  struct CBSTnode *freenode_ptr;
  while (1) {
    if (thisnode_ptr->left == NULL) {
      if (thisnode_ptr->right == NULL) {
        freenode_ptr = thisnode_ptr;
        thisnode_ptr = thisnode_ptr->parent;
        free(freenode_ptr);
        if (thisnode_ptr == NULL) break;
        if (thisnode_ptr->left == freenode_ptr) {
          thisnode_ptr->left = NULL;
        } else {
          thisnode_ptr->right = NULL;
        }
      } else {
        thisnode_ptr = thisnode_ptr->right;
      }
    } else {
      thisnode_ptr = thisnode_ptr->left;
    }
  }
  *root = NULL;
}

_Bool CountingBSTinsert(struct CBSTnode **root, int32_t CBSTix, uint32_t *lt, uint32_t *eq, uint32_t *gt, uint32_t valfreq, int64_t val) {
// Inserts val valfreq times.
// Populates the number of elements in the tree lt, eq to, or gt val after insert.
// Returns true on success.
// Capacity 2^31 - 1 vals.
// No self balancing.
// If you know a good upper bound for the number of nodes required, 
//   make an array of CountingBSTnode_t and then call this function with incrementing CBSTix from 0.
// Otherwise, make sure root starts at NULL and call with CBSTix = -1.
  uint32_t maxcapacity = 0x7fffffff;
  if (valfreq == 0) return false;
  if (valfreq > maxcapacity) return false;
  struct CBSTnode *thisnode_ptr;
  *lt = 0;
  *eq = 0;
  *gt = 0;
  if (*root == NULL) {
    if (CBSTix >= 0) return false; 
    *root = malloc(sizeof(CountingBSTnode_t));
    thisnode_ptr = *root;
    thisnode_ptr->parent = NULL;
    thisnode_ptr->left = NULL;
    thisnode_ptr->right = NULL;
    thisnode_ptr->val = val;
    thisnode_ptr->lt = 0;
    thisnode_ptr->eq = valfreq;
    thisnode_ptr->gt = 0;
    *eq = valfreq;
    return true;
  }
  thisnode_ptr = *root;
  if (CBSTix == 0) {
    thisnode_ptr->parent = NULL;
    thisnode_ptr->left = NULL;
    thisnode_ptr->right = NULL;
    thisnode_ptr->val = val;
    thisnode_ptr->lt = 0;
    thisnode_ptr->eq = valfreq;
    thisnode_ptr->gt = 0;
    *eq = valfreq;
    return true;
  }
  if ((valfreq + thisnode_ptr->lt + thisnode_ptr->eq + thisnode_ptr->gt) > maxcapacity) return false;
  while (true) {
    if (thisnode_ptr->val == val) {
      thisnode_ptr->eq += valfreq;
      *lt += thisnode_ptr->lt;
      *eq = thisnode_ptr->eq;
      *gt += thisnode_ptr->gt;
      return true;
    }
    if (thisnode_ptr->val < val) {
      thisnode_ptr->gt += valfreq;
      *lt += thisnode_ptr->eq + thisnode_ptr->lt;
      if (thisnode_ptr->right == NULL) {
        if (CBSTix >= 0) {
          thisnode_ptr->right = *root + CBSTix;
        } else {
          thisnode_ptr->right = malloc(sizeof(CountingBSTnode_t));
        }
        thisnode_ptr->right->parent = thisnode_ptr;
        thisnode_ptr = thisnode_ptr->right;
        thisnode_ptr->left = NULL;
        thisnode_ptr->right = NULL;
        thisnode_ptr->val = val;
        thisnode_ptr->lt = 0;
        thisnode_ptr->eq = valfreq;
        thisnode_ptr->gt = 0;
        *eq = valfreq;
        return true;
      } else {
        thisnode_ptr = thisnode_ptr->right;
      }
    } else {
      thisnode_ptr->lt += valfreq;
      *gt += thisnode_ptr->eq + thisnode_ptr->gt;
      if (thisnode_ptr->left == NULL) {
        if (CBSTix >= 0) {
          thisnode_ptr->left = *root + CBSTix;
        } else {
          thisnode_ptr->left = malloc(sizeof(CountingBSTnode_t));
        }
        thisnode_ptr->left->parent = thisnode_ptr;
        thisnode_ptr = thisnode_ptr->left;
        thisnode_ptr->left = NULL;
        thisnode_ptr->right = NULL;
        thisnode_ptr->val = val;
        thisnode_ptr->lt = 0;
        thisnode_ptr->eq = valfreq;
        thisnode_ptr->gt = 0;
        *eq = valfreq;
        return true;
      } else {
        thisnode_ptr = thisnode_ptr->left;
      }
    }
  }
}

