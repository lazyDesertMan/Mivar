#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <QString>
#include <QDomDocument>
#include "../models/MivarModel.h"

class ModelLoader {
    static const QString MODEL_TAG;            //!< Тег модели
    static const QString CLASS_TAG;            //!< Тег класса
    static const QString CLASS_LIST_TAG;       //!< Тег списка классов
    static const QString PARAM_TAG;            //!< Тег параметра
    static const QString PARAM_LIST_TAG;       //!< Тег списка параметров
    static const QString RELATION_TAG;         //!< Тег отношения
    static const QString RELATION_LIST_TAG;    //!< Тег списка отношений
    static const QString RULE_TAG;             //!< Тег правила
    static const QString RULE_LIST_TAG;        //!< Тег списка правил
    static const QString CONSTRAINT_LIST_TAG;  //!< Тег списка ограничений

    static const QString ID_ATTRIBUTE;                //!< Атрибут ID
    static const QString NAME_ATTRIBUTE;              //!< Атрибут имени
    static const QString DESCRIPTION_ATTRIBUTE;       //!< Атрибут описания
    static const QString PARAM_TYPE_ATTRIBUTE;        //!< Атрибут типа параметра
    static const QString PARAM_DEFAULT_VAL_ATTRIBUTE; //!< Атрибут значения по умолчанию для параметра
    static const QString RELATION_TYPE_ATTRIBUTE;     //!< Атрибут типа отношения
    static const QString RELATION_INPUTS_ATTRIBUTE;   //!< Атрибут входных параметров отношения
    static const QString RELATION_OUTPUTS_ATTRIBUTE;  //!< Атрибут выходных параметров отношения
    static const QString RULE_INPUTS_ATTRIBUTE;       //!< Атрибут входных параметров правила
    static const QString RULE_OUTPUTS_ATTRIBUTE;      //!< Атрибут выходных параметров правила
    static const QString RULE_RELATION_ATTRIBUTE;     //!< Атрибут ID отношения, связанного с правилом

    static const QString RELATION_TYPE_SIMPLE;        //!< Простое отношение
    static const QString RELATION_TYPE_CONSTRAINT;    //!< Отношение-ограничение
    static const QString RELATION_TYPE_IFCLAUSE;      //!< Условное отношение
    static const QString RELATION_TYPE_FUNCTION;      //!< Сложное отношение (функция на JS)

    /**
     * @brief Загрузка атрибутов модели (ID, имя, описание) 
     *
     * @param model Модель 
     * @param modelNode Узел, содержащий атрибуты модели 
     */
    static void readModelData(std::shared_ptr<MivarModel>& model, const QDomNode& modelNode);

    /**
     * @brief Загрузка параметров сласса
     * 
     * @param mivarClass Класс, для которого загружаются параметры
     * @param paramsNode Узел, содержащий список параметров
     */
    static void loadClassParams(std::shared_ptr<MivarClass>& mivarClass, const QDomNode& paramsNode);

    /**
     * @brief Загрузка отношений в модель
     * 
     * @param model Модель, для которой загружаются отношения
     * @param relationsNode Узел, содержащий список отношений
     */
    static void loadRelations(std::shared_ptr<MivarModel>& model, const QDomNode& relationsNode);

    /**
     * @brief Связывание имён параметров правила с ID параметров классов 
     * 
     * @param rule Правило, в которое загружаются и связываются параметры
     * @param params Строка, содержащая список имён параметров правила и ID связанных с ними параметров классов  
     */
    static void bindRuleParams(std::shared_ptr<MivarRule>& rule, const QString& params);

    /**
     * @brief Загрузка правил в класс
     * 
     * @param model Модель (требуется для связывания правила с отношением)
     * @param mivarClass Класс, в который загружаются правила 
     * @param rulesNode Узел, содержащий список правил
     */
    static void loadRules(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& mivarClass, const QDomNode& rulesNode);

    /**
     * @brief Загрузка подклассов для класса
     * 
     * @param model Модель (необходима для связывания правил класса с отношениями)
     * @param parentClass Класс, для которого загружаются подклассы
     * @param classesNode Узел, содержащий список подклассов
     */
    static void loadSubclasses(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& parentClass, const QDomNode& classesNode);

    /**
     * @brief Загрузка данных класса
     * 
     * @param model Модель (необходима для связывания правил класса с отношениями)
     * @param mivarClass Класс, в который загружаются данные
     * @param classesNode Узел, содержащий данные о классе
     */
    static void loadClass(const std::shared_ptr<MivarModel>& model, std::shared_ptr<MivarClass>& mivarClass, const QDomNode& classesNode);

    /**
     * @brief Сохранение информации о параметре класса
     * 
     * @param doc XML документ
     * @param param Сохраняемый параметр
     * @param parentNode Родительский узел
     */
    static void saveParam(QDomDocument& doc, std::shared_ptr<MivarParam>& param, QDomNode& parentNode);

    /**
     * @brief Сохранение информации о правиле
     * 
     * @param doc XML документ
     * @param rule Сохраняемое правило
     * @param parentNode Родительский узел
     */
    static void saveRule(QDomDocument& doc, std::shared_ptr<MivarRule>& rule, QDomNode& parentNode);

    /**
     * @brief Сохранение информации о классе
     * 
     * @param doc XML документ
     * @param mivarClass Сохраняемый класс
     * @param parentNode Родительский узел
     */
    static void saveClass(QDomDocument& doc, std::shared_ptr<MivarClass>& mivarClass, QDomNode& parentNode);

    /**
     * @brief Сохранение информации об отношении
     * 
     * @param doc XML документ
     * @param relation Сохраняемое отношение
     * @param parentNode Родительский узел
     */
    static void saveRelation(QDomDocument& doc, std::shared_ptr<MivarRelation>& relation, QDomNode& parentNode);
public:
    /**
     * @brief Загрузка модели из файла формата XML
     * 
     * @param path Путь к файлу, из которого проводится загрузка
     * @return std::shared_ptr<MivarModel> Загруженная модель (в случае, если модель не удалось загрузить, ID = "")
     */
    static std::shared_ptr<MivarModel> load(const QString& path);

    /**
     * @brief Сохранение модели в файл
     * 
     * @param model Сохраняемая модель
     * @param path Путь до файла формата XML
     */
    static void save(const std::shared_ptr<MivarModel>& model, const QString& path);
};

#endif
