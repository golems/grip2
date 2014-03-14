#ifndef TREEVIEWRETURN_H
#define TREEVIEWRETURN_H

#include <QMetaType>

enum DataType
{
    Return_Type_Robot,
    Return_Type_Node
};

/**
 * \class TreeViewReturn tree_view.h
 * \brief This class is used to return the object that is selected in the
 * Object Explorer or Tree View.
 */
class TreeViewReturn
{
public:
    /**
     * \brief Constructor for TreeViewReturn
     */
    TreeViewReturn(){}

    /// used to store the Skeleton object or the BodyNode object
    void* object;

    /// used to differentiate between the skeleton object and BodyNode object
    DataType dType;
};
Q_DECLARE_METATYPE(TreeViewReturn*);

#endif // TREEVIEWRETURN_H
