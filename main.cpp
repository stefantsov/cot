#include <iostream>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "cot/connection.h"
#include "cot/sql.h"
#include "cot/author.h"

using namespace std;
using namespace cot;

void perform()
{
    Connection::connect("localhost", "cot_user", "cot_pass", "cot_db");
    
    // An example of INSERT query.
    boost::scoped_ptr<Author> author(new Author());
    author->name = "John Smith";
    author->age = 19;
    author->bookCount = 0;
    author->save();
    
    cout << "Just saved an author with id = " << author->id << endl;
    
    // An example of UPDATE query.
    vector< boost::shared_ptr<Author> > authors = Select< Author, All >::with(0);
    for (int i = 0; i < (int)authors.size(); ++i) {
        authors[i]->age += 1;
        authors[i]->save();
    }
    cout << "All authors have grown older." << endl;

    // There are several things to note:

    // 1. Static variable _age_ is the metadatum describing the instance
    // variable age of class Author. Lowdashes (_) attached, for there
    // could be no instance variables and static variables with the same
    // name. (NOTE: same applies to _name_ variable)

    // 2.  It seems that the C++ 98 standard prohibits the usage of
    // values as template parameters for non-integer types.  Thus, all
    // query parameters should be passed as arguments to a variadic
    // member function.  The latter should guess the parameters types
    // from the compile-time list of parameter types and sizes.

    // 3.  As it reveals, the variadic functions should have at least
    // one argument with a name.  Since there is no way to omit the
    // latter, it would be used as argument counter.

    authors = 
        Select< Author, Where<
            Or<
                Lt<Author::_age_>,
                Eq<Author::_name_> > > >::with(2, 41, "John Smith");

    for (int i = 0; i < (int)authors.size(); ++i) {
        cout << authors[i]->id << " | " \
                << authors[i]->name << " | " \
                << authors[i]->age << " | " \
                << authors[i]->bookCount << endl;
    }
    
    // An example of DELETE query.
    authors = Select< Author, Where< Eq<Author::_bookCount_> > >::with(1, 0);
    for (int i = 0; i < (int)authors.size(); ++i) {
        authors[i]->remove();
    }
    cout << "Removed all authors without books." << endl;
}

int main(int argc, char * argv[])
{
    try {
        perform();
    } catch (const std::runtime_error * error) {
        cerr << error->what() << endl;
    }
    
    return 0;
}
