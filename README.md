## MeusProventos

> open source software for helping managing funds in Brazilian system

Sistema livre para simples contabilização de proventos de ações, fundos, etc, através de planilhas em csv.

### Objetivos

O objetivo desse software livre é permitir, de forma segura e gratuita, uma rápida conferência para proventos de ações, fundos, etc, para um determinado período.

### Informações Importantes

- A estrutura dos dados em colunas tem formato compatível com o utilizado por outros sistemas (como de exportação de movimentação na bolsa B3 [pelo menos em Maio/2022...]).
- À medida em que o tempo passa, mudanças podem (e devem) ser necessárias, então, CONTRIBUA.
- O sistema tem código aberto, com licença MIT, SEM NENHUMA GARANTIA.
- Somente utilize esse sistema para simples conferência de proventos pessoais, sempre observando valores oficiais nos informes de rendimentos, bem como demais sistemas profissionais de gestão de ativos pessoais
- **Esse sistema se encontra em versão ALPHA, ou seja, não contempla todas funcionalidades desejadas, e mesmo as que existem, estão fortemente sujeitas a erro!**
- Utilize como curioso ou para aprendizado pessoal... caso seja desenvolvedor C/C++ e queira ajudar a melhorá-lo, fique à vontade!

-----

### Exemplo Rápido

Para construir e executar o software em C/C++

```
make
make test
./meusproventos --entrada exemplo/exemplo0.csv
```


-----

### Formato de Entrada

Existem dois arquivos principais: planilha de movimentações e planilha de posições.

#### Planilha de movimentações (entrada)

São esperadas 8 colunas (sem checagem de título) na planilha de entrada:

1. Entrada e Saída: valor "Credito" ou "Debito"
1. Data da movimentação: formato DD/MM/AAAA
1. Tipo de Operação: "Rendimento", etc
1. Nome do Produto: "AABB3 - Nome"
1. Nome da Corretora: "Corretora XYZ"
1. Quantidade: número positivo (de cotas ou ações)
1. Preço unitário: formato "R$XXX,YY"
1. Valor da Operação: formato "R$XXX,YY" (aproximadamente ~ Quantidade x Preço Unitario)


-----

### Exemplo Didático

Exemplo didático inspirado em [Como Declarar Ações - Infomoney (2022)](https://www.infomoney.com.br/guias/declarar-acoes-imposto-de-renda-ir/).

-----

### Licença

Dependências: `vincentlaucsb/csv.hpp` (MIT License), [HowardHinnant/date](https://github.com/HowardHinnant/date/) (MIT License).

Copyleft 2022 - MIT License

MeusProventos team