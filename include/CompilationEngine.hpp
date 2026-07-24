#ifndef COMPILATIONENGINER_HPP
#define COMPILATIONENGINER_HPP

#include "Tokenizer.hpp"

class CompilationEngine
{

  private:
    Tokenizer *tokenizer;
    std::ofstream output;

  public:
    /**
     * Constructor / Initializer:
     * Creates a new compilation engine with the given input (JackTokenizer)
     * and output stream/file. The next routine called must be compileClass().
     */
    CompilationEngine(Tokenizer *inputTokenizer,
                      const std::string &outputFileName);

    /**
     * Destructor:
     * Closes the output file stream.
     */
    ~CompilationEngine();

    // --- Program Structure ---

    /** Compiles a complete class. */
    void compileClass();

    /** Compiles a static variable declaration or a field declaration. */
    void compileClassVarDec();

    /** Compiles a complete method, function, or constructor. */
    void compileSubroutine();

    /**
     * Compiles a (possibly empty) parameter list.
     * Does not handle the enclosing parentheses tokens '(' and ')'.
     */
    void compileParameterList();

    /** Compiles a subroutine's body. */
    void compileSubroutineBody();

    /** Compiles a var declaration. */
    void compileVarDec();

    // --- Statements ---

    /**
     * Compiles a sequence of statements.
     * Does not handle the enclosing curly bracket tokens '{' and '}'.
     */
    void compileStatements();

    /** Compiles a let statement. */
    void compileLet();

    /** Compiles an if statement, possibly with a trailing else clause. */
    void compileIf();

    /** Compiles a while statement. */
    void compileWhile();

    /** Compiles a do statement. */
    void compileDo();

    /** Compiles a return statement. */
    void compileReturn();

    // --- Expressions ---

    /** Compiles an expression. */
    void compileExpression();

    /**
     * Compiles a term. If the current token is an identifier, the routine
     * must resolve it into a variable, an array entry, or a subroutine call.
     * A single lookahead token ('[', '(', or '.') suffices to distinguish.
     */
    void compileTerm();

    /**
     * Compiles a (possibly empty) comma-separated list of expressions.
     * @return The number of expressions in the list.
     */
    int compileExpressionList();
};

#endif // COMPILATIONENGINER_HPP
