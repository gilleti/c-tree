
/*
 * Basic (limited), memory efficient, binary tree for Python. Data type is uint_64.
 */
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define MAXNODES 10  // Max number of nodes in the tree. Exceeding this will yield an exception.

// We refer to node storage locations by means of an index rather than a pointer in order to save memory.
// E.g.: The root node is stored at root[0].
typedef uint32_t nodeix_t;
#define NODEIX_NULL       0
#define NODEIX_FIRSTNODE  1
struct btree_node_t {
  uint64_t data;
  nodeix_t lt;
  nodeix_t gt;
};


static nodeix_t rootnodeix;     // Index of the root node
static nodeix_t lastnodeix;     // Index of the last allocated node
static struct btree_node_t *tree;      // Tree memory


static PyObject *
btree_insert(PyObject *self, PyObject *args)
{
    uint64_t data;
    
    if (!PyArg_ParseTuple(args, "l", &data)) {
      printf("Btree insert: failed to parse argument\n");
      return NULL;
    }

    // Search the tree for data
    if (rootnodeix == NODEIX_NULL) {
      // Tree is emppty. Create root node.
      lastnodeix = rootnodeix = NODEIX_FIRSTNODE;
      tree[rootnodeix].data = data;
      tree[rootnodeix].lt = NODEIX_NULL;
      tree[rootnodeix].gt = NODEIX_NULL;
    }
    else {
      // Tree not empty. Search starting at the root node.
      nodeix_t currnode = rootnodeix;
      nodeix_t *parentnode_childix;
      
      while(currnode != NODEIX_NULL) {
	if (tree[currnode].data == data) {
	  // Current node data is equal to data to be inserted. Don't insert anything and return 0.
	  return PyLong_FromLong(0);
	}
	else {	  
	  if (tree[currnode].data > data) {
 	    // Data to be inserted is less than this node's data. Go to left (less than) child.
	    parentnode_childix = &tree[currnode].lt;
    	    currnode = tree[currnode].lt;
	  }
	  else {
	    // Data to be inserted is less than this node's data. Go to right (greater than) child.
	    parentnode_childix = &tree[currnode].gt;
    	    currnode = tree[currnode].gt;
	  }     
	}
      }

      // Search has finished. Create a new leaf node for data.
      // Allocate new space for the node
      if ((lastnodeix - rootnodeix) + NODEIX_FIRSTNODE >= MAXNODES) {
	// Tree is full. Can't insert any more nodes! Yield an exception.
	// TODO fix ptyhon exception
	printf("btree insert: Tree is full!\n");
	return NULL;
      }
      
      lastnodeix++;
      tree[lastnodeix].data = data;
      tree[lastnodeix].lt = NODEIX_NULL;
      tree[lastnodeix].gt = NODEIX_NULL;

      // Connect it to the parent
      *parentnode_childix = lastnodeix;
    }

    return PyLong_FromLong((lastnodeix - rootnodeix) + NODEIX_FIRSTNODE);
}
    
    
    //printf("Btree: Got data %lu. root is %lu\n", data, *root);
    //*(root + lastnode * sizeof(uint64_t)) = data;
    //++lastnode;
    
	   //   for (uint64_t nodec = 0; nodec < lastnode; ++nodec)
	   //printf("At %lu#: %lu\n", nodec, *(root + nodec * sizeof(uint64_t))); 
    


static PyMethodDef BtreeMethods[] = {
   
    {"insert",  btree_insert, METH_VARARGS, "Insert an integer (stored as uint_64) in the tree. Yields error if it already exists."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef btreemodule = {
    PyModuleDef_HEAD_INIT,
    "btree",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    BtreeMethods
};

PyMODINIT_FUNC
PyInit_btree(void)
{
  // Allocate memory for tree. Note: The first NODEIX_FIRTSNODE indexes are unused.
  tree = malloc(sizeof(struct btree_node_t) * (MAXNODES + NODEIX_FIRSTNODE));
  lastnodeix = rootnodeix = NODEIX_NULL;
  //  *(root + lastnode * sizeof(uint64_t)) = 555;
  //++lastnode;
  //printf("Tree init done. uint64_t is of size %lu\n", sizeof(uint64_t));
  return PyModule_Create(&btreemodule);
}
