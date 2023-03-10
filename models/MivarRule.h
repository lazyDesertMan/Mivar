#ifndef MIVAR_RULE_H
#define MIVAR_RULE_H

#include <map>
#include "IObserver.h"
#include "MivarRelation.h"
#include "MivarObject.h"

class MivarRule : public MivarObject, public IObserver, public std::enable_shared_from_this<MivarRule> {
    void syncWithRelation();
    std::shared_ptr<MivarRelation> m_relation;
    std::map<QString, QString> m_params;
    bool m_isBindet;

public:
    MivarRule() : m_isBindet(false) {}
    MivarRule(MivarRule&) = delete;
    MivarRule(MivarRule&&) = delete;
    MivarRule& operator=(MivarRule&) = delete;
    ~MivarRule();

    void bindRelation(std::shared_ptr<MivarRelation> relation);
    void unbindRelation();
    const std::shared_ptr<MivarRelation> getBindetRelation() const;

    void handle(int16_t code) override;

    /**
     * @brief Запрос ID связанного параметра по имени
     * 
     * @param paramName Имя параметра
     * @return QString ID параметра
     */
    QString paramID(const QString& paramName);
    
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
