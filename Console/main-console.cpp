#include <iostream>

#include "BouncinSP.h"
#include "Song.h"

#include <boost/lexical_cast.hpp>

using namespace std;

int main(int argc, char **argv)
{
    if ( argc < 3 )
    {
        cout << "params: path_to_sp404sx_folder pattern_idx" << endl;
        return -1;
    }

    Song song(argv[1]);
    bool ret = song.export_pattern( boost::lexical_cast<unsigned>(argv[2]) );

    cout << "ret: " << (int) ret << endl;

    return 0;
}

