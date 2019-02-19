#pragma once

#include <memory>

template<typename Builder>
struct BuilderTraits;

template<typename Builder>
class AbstractBuilder
{
public:
    using Options = typename BuilderTraits<Builder>::Options;
    using Component = typename BuilderTraits<Builder>::Component;
    using DefaultHandler = typename BuilderTraits<Builder>::DefaultHandler;
    using IDefaultHandler = typename DefaultHandler::interface; // FIXME temporarily

    AbstractBuilder() = default;
    virtual ~AbstractBuilder() = default;

    std::unique_ptr<Component> build()
    {
        auto component = create();
        doSetup(*component);
        return component;
    }

    Builder& options(const Options& opts)
    {
        return retrieveOptions(opts);
    }

    Builder& adaptor(std::unique_ptr<IDefaultHandler>&& adaptor)
    {
        m_adaptor = std::move(adaptor);
        return static_cast<Builder&>(*this);
    }

protected:
    virtual Builder& retrieveOptions(const typename BuilderTraits<Builder>::Options& opts) = 0;
    virtual std::unique_ptr<Component> create() = 0;
    virtual void doSetup(typename BuilderTraits<Builder>::Component&) { }

    std::unique_ptr<IDefaultHandler> createHandler()
    {
        if(!m_adaptor) return std::make_unique<DefaultHandler>();

        return std::move(m_adaptor);
    }

protected:
    std::unique_ptr<IDefaultHandler> m_adaptor;

};
