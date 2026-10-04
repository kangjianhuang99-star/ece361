# AI Usage

I used OpenAI ChatGPT (Codex) for step-by-step explanations of C bit operations, help drafting the implementation and tests, and debugging compiler errors.

During review, I found that an AI-generated test summary line used `\\n` where C needs `\n`. That would print the characters `\n` instead of starting a new line. I corrected the string and then ran `make test`.

