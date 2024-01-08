


set(source
    differentiate/differentiate.c differentiate/functions.c
    draw/draw.c
    simplify/simplify.c simplify/term.c
    tree/expression.c tree/token.c tree/tree.c

    derivative.c
)

target_sources(${PROJECT_NAME} PUBLIC ${source})
