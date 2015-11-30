#ifndef REPORT_H_
#define REPORT_H_

#include <string>

#include "directories_configuration.h"
#include "instances.h"
#include "tree_configuration.h"

class Report {

    public:
        Report(Instances* instances, DirectoriesConfiguration* dir_conf):
            instances_(instances), dir_conf_(dir_conf) { };
        ~Report() { };

        virtual void GenerateReport() const = 0;

    protected:
        Instances* instances_;
        DirectoriesConfiguration* dir_conf_;

        void GenerateBeginning(FILE* f, std::string title) const;
        void GenerateEnd(FILE* f) const;
        void GenerateDataInformation(FILE* f,
                TreeConfiguration* tree_conf) const;
};

#endif    // END_REPORT_H_
