#include "fvaItem.h"

#include <algorithm>
#include <any>
#include <ctime>
#include <string>
#include <vector>

#include "fvafile.h"
#include "fvafolder.h"
#include "fva_qt_port_2_stl.h"


fvaItem::fvaItem() {
    isFiltered = true;
    type = FVA_FS_TYPE_UNKNOWN;
    pFvaFolder = nullptr;
    pFvaFile = nullptr;
}

fvaItem::~fvaItem() {
    for (auto idChild = children.begin(); idChild != children.end(); ++idChild) {
        if (*idChild == nullptr) continue;
        delete *idChild;
        *idChild = nullptr;
    }

    if (nullptr != pFvaFolder) delete pFvaFolder;

    if (nullptr != pFvaFile) delete pFvaFile;
}
std::string fvaItem::getGuiName(const std::map<std::string, std::any>& dictionaries) {
    if (type == FVA_FS_TYPE_DIR) {
        std::string desc;
        if (pFvaFolder && pFvaFolder->eventId != 0) {
            const DictRows* events = getDictionaryRows(dictionaries, "events");
            if (events != nullptr) {
                for (const auto& row : *events) {
                    if (toUnsigned(getRowValue(row, "ID")) == pFvaFolder->eventId) {
                        desc = " - " + getRowValue(row, "fullName");
                        break;
                    }
                }
            }

            if (pFvaFolder && !pFvaFolder->eventReasonPeopleIds.empty() && pFvaFolder->eventReasonPeopleIds[0]) {
                const DictRows* people = getDictionaryRows(dictionaries, "people");
                if (people != nullptr) {
                    for (const auto& row : *people) {
                        if (toUnsigned(getRowValue(row, "ID")) == pFvaFolder->eventReasonPeopleIds[0]) {
                            desc += "," + getRowValue(row, "name");
                            break;
                        }
                    }
                }
            }
        }

        if (isValidDate(dateTo)) {
            if (dateFrom == dateTo)  // one year
                return formatDateTime(dateFrom, "yyyy") + desc;
            else if (isOneDayAfter(dateFrom, dateTo))
                return formatDateTime(dateFrom, "yyyy/MM/dd") + desc;
            else
                return formatDateTime(dateFrom, "yyyy/MM/dd") + formatDateTime(dateTo, "-yyyy/MM/dd") + desc;
        } else {
            return formatDateTime(dateFrom, "yyyy/MM/dd") + desc;
        }
    } else {
        return formatDateTime(dateFrom, "yyyy-MM-dd (hh:mm:ss)");
    }
}

std::string fvaItem::getGuiFullName(const std::map<std::string, std::any>& dictionaries) {
    std::string fullName;
    if (!pFvaFolder && !pFvaFile) return "";
    if (type != FVA_FS_TYPE_DIR && pFvaFile) {
        if (!pFvaFile->description.empty()) fullName = pFvaFile->description;
    }

    if (type != FVA_FS_TYPE_DIR && pFvaFile) {
        if (fullName.empty())
            fullName = pFvaFile->comment;
        else
            fullName += ", " + pFvaFile->comment;
    } else if (type == FVA_FS_TYPE_DIR && pFvaFolder) {
        if (fullName.empty())
            fullName = pFvaFolder->tags;
        else
            fullName += ", " + pFvaFolder->tags;
    }

    if (type != FVA_FS_TYPE_DIR && pFvaFile) {
        fillNameByOneId(pFvaFile->deviceId, "devices", dictionaries, fullName);
    }
    return fullName;
}
