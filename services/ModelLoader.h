#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <QString>
#include <QDomDocument>
#include "../models/MivarModel.h"

class ModelLoader {
    static void readModelData(std::shared_ptr<MivarModel>& model, const QDomNode& modelNode);
    static void loadClassParams(std::shared_ptr<MivarClass>& mivarClass, const QDomNode& paramsNode);
    static void loadRelations(std::shared_ptr<MivarModel>& model, const QDomNode& relationsNode);
    static void bindRuleParams(std::shared_ptr<MivarRule>& rule, const QString& params);
    static void loadRules(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& mivarClass, const QDomNode& rulesNode);
    static void loadSubclasses(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& parentClass, const QDomNode& classesNode);
    static void loadClass(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& mivarClass, const QDomNode& classesNode);
public:
    static std::shared_ptr<MivarModel> load(const QString& path);
    static void save(const MivarModel& model, const QString& path);
};

#endif
