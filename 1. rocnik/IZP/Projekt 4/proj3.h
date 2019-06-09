/**
 * @mainpage Project 3 - Simple Cluster analysis
 * @author Ondrej Studnicka (xstudn00)
 * @date 13. 12. 2017
 * @file proj3.h
 * @brief Simple cluster analysis
 */

/**
 * @defgroup Structures
 * @{
 */

/**
 * @brief Object structure
 * Attributes: id, coordinate x, coordinate y
 */
struct obj_t {
	int id;
	float x;
	float y;
};

/**
 * @brief Structure of clusters of objects
 * Attributes: size, capacity, object structure
 */
struct cluster_t {
	int size;
	int capacity;
	struct obj_t *obj;
};

/**@}*/


/**
 * @defgroup object_editing Creating and editting objects
 * @{
 */

/**
 * @brief Function allocates the memory for the object
 *
 * @details Initialize the cluster 'c'. It allocates the memory for the cap of the object (capacity).
 * The NULL pointer for an object array indicates a capacity of 0.
 *
 * @param pointer to cluster 'c'
 * @param capacity 'cap'
 *
 * @post cluster 'c' will have allocated memory
 */

void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Function clear all clusters
 *
 * @details Function removes all objects and initialize to empty cluster.
 *
 * @param pointer to cluster 'c'
 *
 * @post Allocated memory will be released.
 */

void clear_cluster(struct cluster_t *c);

/**
 * @brief Constant CLUSTER_CHUNK
 * @details Chunk of cluster objects. Value recommended for reallocation.
 */

extern const int CLUSTER_CHUNK;

/**
 * @brief Function changes the cluster capacity 'c'.
 *
 * @details Change cluster capacity 'c' to 'new_cap' capacity.
 *
 * @param pointer to cluster 'c'
 * @param new capacity 'new_cap'
 *
 * @pre Capacity of cluster 'c' is greater or equal to zero
 *
 * @post Capacity of cluster 'c' will be changed to a new value
 *
 * @return cluster with new capacity is returned, in an error occures - NULL is returned
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Function add object 'obj' to the end of the cluster
 *
 * @details Function add object 'obj' to the end of the cluster. Function expands the cluster if the object does not fit.
 *
 * @param pointer to cluster 'c'
 * @param object 'obj'
 *
 * @pre Number of objects in the cluster 'c' is greater or equal to zero.
 *
 * @post At the last cluster position, the object is added.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Function merges two clusters in one and clusters will be sorted by their ID
 *
 * @details Function adds objects from cluster 'c2' into cluster 'c1'. The cluster 'c1' will be expanded if necessary.
 * Objects in cluster 'c1' will be sorted by their ID. Cluster 'c2' will be unchanged.
 *
 * @param pointer to cluster 'c1'
 * @param pointer to cluster 'c2'
 *
 * @pre Number of objects in the cluster 'c2' is greater than zero.
 *
 * @post Cluster 'c1' will be expanded by objects from cluster 'c2'. Objects in cluster 'c1' will be sorted by their ID.
 */

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);
/**@}*/

/**
 * @defgroup array_of_clusters Creating and editting arrays
 * @{
 */

/**
 * @brief Function removes cluster from an arraz
 * @details Function removes cluster from array of clusters (name - 'carr', size 'narr'). The cluster to be removed is on the idx index.
 * New size of array is narr-1.
 *
 * @param pointer to array 'carr'
 * @param number of clusters 'narr'
 * @param index of removed cluster 'idx
 *
 * @post From the array 'carr' will be cluster on 'idx' removed. New size of array is narr-1.

 * @return New number of clusters in arraz (narr-1)
 */

int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief Function calculates distance
 *
 * @details Function calculates Euclidean distance between two objects.
 *
 * @param pointer to object 'o1'
 * @param pointer to object 'o2'
 *
 * @return distance is returned
 */

float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Function calculates distance of two objects
 *
 * @details Function calculates distance of two objects
 *
 * @param pointer to cluster 'c1' object
 * @param pointer to cluster 'c2' object
 *
 * @pre Number of objects in both clusters will be greater than zero.
 *
 * @return distance between clusters 'c1' and 'c2'
 */

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Function find two closest clusters
 *
 * @details Function find two closest clusters. In array 'carr' of size 'narr' find two closest clusters. Function stores index of founded clusters
 * in memory on adress 'c1' and 'c2'.
 *
 * @param pointer to array 'carr'
 * @param Number of clusters in array 'narr'
 * @param pointer to index of first founded cluster 'c1'
 * @param pointer to index of first founded cluster 'c2'
 *
 * @post Index of founded clusters are stored in 'c1' and 'c2'
 */

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Functions sorts cluster.
 *
 * @details Functions sorts clusters by their ID.
 *
 * @param pointer to cluster 'c'
 *
 * @post Objects in cluster 'c' will be sorted in ascending order by their ID.
 */

void sort_cluster(struct cluster_t *c);

/**
 * @brief Function prints cluster
 *
 * @details Function prints cluster to stdout
 *
 * @param pointer to cluster 'c'
 *
 * @post Print cluster 'c' to stdout
 */

void print_cluster(struct cluster_t *c);

/**
 * @brief Function loads objects in file
 *
 * @details Functions loads objects from file 'filename' For each object create cluster and save it in array of clusters.
 * Functions allocate memory for array of all clusters and the pointer to the first item in array is stored in memory as 'arr'.
 * Function returns number of loaded objects. In the event of any error, the NULL value is stored in the memory where 'arr' is referenced.
 *
 * @param pointer to name of file 'filename'
 * @param pointer to an array of clusters 'arr'

 * @pre File 'filename' will be exist. The file will be formatted correctly.
 *
 * @post Every object in file will be created cluster. All clusters will be stored in array of clusters 'arr'. Array will have allocated memory.

 * @return number of loaded lines
 */

int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief Functions print clusters
 *
 * @details Functions prints array of clusters.
 *
 * @param pointer to first item (cluster) 'carr'
 * @param number of printed clusters 'narr'
 *
 * @post Clusters will be printed on stdout.
 */
void print_clusters(struct cluster_t *carr, int narr);


/**@}*/

/**
 * @defgroup functions_created_by_me Own function
 * @{
 */

/**
 * @brief Function checks if argument is number or not
 * @details Function checks if argument is number or not
 *
 * @param pointer argument for check 'word'
 *
 * @pre Program is running and any arguments were entered.
 *
 * @return zero is returned if argument is number, else 1
 */


int isNumber(char* word);


/**@}*/