# planilha_ncurses - Trabalho de Oficina de Computação (Prof Carlos Maziero)

Este projeto consiste em construir uma planilha eletrônica em modo texto, usando a biblioteca NCurses para a gestão do terminal. A interface da planilha deve ser similar à figura abaixo:

![Planilha](planilha.png?raw=true "Planilha")

### Funcionalidades
- ler e escrever arquivos em formato CSV (deve ser capaz de ler/escrever arquivos simples compatíveis com o LibreOffice Calc ou Microsoft Excel);
- suportar matrizes de até 100 linhas × 26 colunas (a tela deve “deslizar” na horizontal e vertical para mostrar parte visível da planilha);
- cada coluna da planilha ocupa 8 caracteres na tela;
- implementar corte/cópia/cola de grupos de células contíguas;
- ser capaz de calcular fórmulas simples;
- se adaptar às dimensões do terminal ao iniciar.

### Formato CSV
- campos delimitados por vírgula
- linhas delimitadas por enter (\n)
- campos representados por valores textuais (não em binário)
- qualquer valor/campo pode ser envolto por aspas duplas (“)
- campos contendo vírgulas devem ser envoltos por aspas duplas
- números reais usam ponto ”.“ para a parte fracionária
- cada campo pode ter até 255 caracteres (mostrar até 8 na tela)
- ignorar campos contendo “\n” ou aspas como parte do valor

### Valores nas células
- números inteiros e reais, positivos e negativos
- strings
- fórmulas simples: strings no formato = A op B, onde A e B são números ou referências de células e op é uma operação algébrica simples (+, -, *, /).
- Células podem ser referenciadas usando suas coordenadas: por exemplo, a string A5 referencia o valor da célula na coluna A e linha 5.

### Controles
- Setas (←↑↓→): mover cursor (para selecionar a célula ativa)
- Enter: editar a célula corrente (selecionada)
- ^S : salvar arquivo corrente
- ^R: ler novo arquivo
- ^space : fixar âncora (define um retângulo junto com o cursor)
- ^C: copiar retângulo para o clipboard
- ^X: cortar retângulo para o clipboard
- ^V: colar conteúdo do clipboard a partir da posição do cursor
- ^Q: encerrar o programa (quit); perguntar se deseja salvar antes!

### Forma de chamada
- texcel : abre uma planilha em branco (sem nome)
- texcel arq1.csv: abre o arquivo arq1.csv
