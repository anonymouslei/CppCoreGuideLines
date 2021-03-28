# In: Introduction
## In.target: Target readership
the rules emphasize static type and resource safety. for that reason, they emphasize possibilities for range checking, for avoiding dereferencing `nullptr`, for avoiding dangling pointers, and the systematic use of exceptions (via RAII). Partly to chieve that and partly to minimize obscure code as a source of errors, the rules also emphasize simplicity and the hiding of necessary complexity behind well-specified interfaces.

## In.aims: Aims
## In.not: Non-aims
## In.force: Enforcement
where appropriate, we label a rule (in the #Enforcement# sections) with the name of groups of related rules(called "profiles"). A rule can be part of several profiles ,or none.
## In.struct: The structure of this document
## In.sec: Major sections