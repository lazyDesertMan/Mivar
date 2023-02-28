#ifndef MIVAR_RULE_H
#define MIVAR_RULE_H

#include <map>
#include "IObserver.h"
#include "MivarRelation.h"
#include "MivarObject.h"

class MivarRule : public MivarObject, public IObservable {
    struct MivarRuleLogic : public IObserver {
        std::shared_ptr<MivarRelation> relation;
        std::map<QString, QString> params;

        MivarRule* parent;
        void syncWithRelation();
        void handle();
    };
    std::shared_ptr<MivarRuleLogic> m_rule;

public:
    MivarRule(const std::shared_ptr<MivarRelation>& relation);
    MivarRule(MivarRule&) = delete;
    MivarRule(MivarRule&&) = delete;
    MivarRule& operator=(MivarRule&) = delete;
    ~MivarRule();

    bool isCorrect();
    /**
     * @brief Привязывает параметр с именем paramName правила
     *        к параметру с ID paramId миварного класса.
     * 
     * @param paramName Имя параметра правила.
     * @param paramId ID параметра класса.
     * @return true Параметр был привязан.
     * @return false Не удалось привязать параметр.
     */
    bool bindParam(const QString& paramName, const QString& paramId);

    const std::map<QString, QString>& params() const noexcept;
};

#endif
