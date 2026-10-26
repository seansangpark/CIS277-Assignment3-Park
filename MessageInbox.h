#ifndef MESSAGE_INBOX_H
#define MESSAGE_INBOX_H

#include <cstddef>
#include <string>
#include <vector>

struct TextMessage
{
    size_t id;
    std::string sender;
    std::string text;
    bool isRead;
};

class MessageInbox
{
public:
    explicit MessageInbox(size_t maxTextBytes);
    ~MessageInbox();

    MessageInbox(const MessageInbox&) = delete;
    MessageInbox& operator=(const MessageInbox&) = delete;

    bool receive(size_t id, const std::string& sender, const std::string& text);
    bool read(size_t id, TextMessage& result);
    bool erase(size_t id);
    std::vector<TextMessage> messages() const;
    void clear();
    size_t messageCount() const;
    size_t usedTextBytes() const;
    size_t remainingTextBytes() const;

private:
    struct Node
    {
        TextMessage message;
        Node* next = nullptr;
    };

    Node* findNode(size_t id) const;

    Node* head;
    Node* tail;
    size_t count;
    size_t usedBytes;
    size_t budget;
};

#endif
