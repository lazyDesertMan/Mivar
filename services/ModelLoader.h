#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <QString>
#include <QDomDocument>
#include "../models/MivarModel.h"

class ModelLoader {
    static void readModelData(MivarModel& model, const QDomNode& modelNode);
    static void loadRelations(MivarModel& model, const QDomNode& relationsNode);
    static void loadSubclasses(MivarModel& model, const QDomNode& classesNode);
    static void loadClass(MivarModel& model, const QDomNode& classesNode);
public:
    static MivarModel load(const QString& path);
    static void save(const MivarModel& model, const QString& path);
};

#endif
