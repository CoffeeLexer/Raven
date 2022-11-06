//
// Created by tartarus on 11/6/22.
//

#ifndef RAVEN_FILESYSTEM_H
#define RAVEN_FILESYSTEM_H

namespace Raven {

    class FileSystem {
    public:
        static std::string getExtension(const std::string &file);
    };

} // Raven

#endif //RAVEN_FILESYSTEM_H
