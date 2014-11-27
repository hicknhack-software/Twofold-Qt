#ifndef TWOFOLD_TEXTLOADER_H
#define TWOFOLD_TEXTLOADER_H

#include <QString>

#include <memory>

namespace Twofold {

class TextLoader; // local forward
using TextLoaderPtr = std::shared_ptr<TextLoader>;

/**
 * @brief Interface for resolving and loading text files
 *
 * You can fetch files from memory or from a network.
 */
class TextLoader
{
public:
    enum Status {
        Success, ///< File was found and loading was sucessfull
        NotFound, ///< File was not found
        ErrorLoading, ///< File was found, error occured while loading it
    };
    struct Result {
        Status status;
        QString name; ///< resolved name (used in logs and sourceMaps) - only valid if found
        QString text; ///< content - empty unless success
    };

public:
    virtual ~TextLoader() {}

    virtual Result load(const QString& name) const = 0;
};

} // namespace Twofold

#endif // TWOFOLD_TEXTLOADER_H
