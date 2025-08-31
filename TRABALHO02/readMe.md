Esta atividade consiste em ler arquivos de metadados e dados para simular um select *. Para tal, serão fornecidos três arquivos (ver zip em anexo): um contendo o metadado da tabela (table.dic), outro o metadado dos atributos (att.dic) e um outro com os dados de uma das tabelas presentes no table.dic.
Implementar uma aplicação em C que faça:
- Ao executar o programa deverá ser passado  um parâmetro (uma sequência de caracteres) correspondente ao nome lógico (tabela) de um arquivo de dados.
- O aplicativo procurará no arquivo table.dic o nome da tabela passado como parâmetro, caso encontrar, deverá abir o arquivo com o nome físico para leitura. A estrutura do arquivo table.dic é a seguinte: 
 * * Um inteiro que corresponde ao id do arquivo
 * * Uma sequência de, no máximo, 20 caracteres que corresponde ao nome lógico do arquivo(nome da tabela), e 
 * * Outra sequência de, no máximo, 20 caracteres que corresponde ao nome físico do arquivo (nome do arquivo no disco).
- Caso o nome lógico for encontrado em table.dic, o arquivo att.dic deve ser aberto para procurar o esquema do arquivo. Ele está organizado da seguinte forma: 
 * * Um inteiro que corresponde ao id do arquivo (chave estrangeira da table.dic) a ser consultado, 
 * * Uma sequência de, no máximo, 20 caracteres que corresponde ao nome do atributo, 
 * * Um caracterer que corresponde ao tipo (S string, I inteiro ou D double), 
 * * Um caracter que indica a opcionalidade do atributo (0 obrigatório, 1 opcional), e 
 * * Um inteiro que indica o tamanho do atributo.

Entregar um zip contendo os fontes criados (caso usarem .h e .c - aconselhável), ou apenas o fonte caso tudo estiver em um arquivo. O nome do zip ou c deverá ser a matrícula do estudante.


 Exemplo:

 table.dic <1, "car", "xyz.dat">, <2, "owner", "d001.dat">

 att.dic <1, "model", 'S', 0, 30>, <1, "year", 'I',0, 4>, <2, "issn", 'S', 0, 12>, <2, "name", 'S', 0, 30>

Se o nome do aplicativo for  selectDB, a execução seria selectDB owner (owner corresponde ao nome da tabela - nome lógico).
O arquivo d001.dat conterá os dados com o esquema correspondente ao id 2 (owner) no arquivo att.dic, ou seja, issn sequência de 12 caracteres, obrigatório; e name sequência de 30 caracteres, obrigatório.

Finalmente, o conteúdo do arquivo físico deve ser listado. Para facilitar, não será necessário tratar atributos opcionais pois todos serão considerados obrigatórios.

 O zip anexo contém um exemplo de table.dic, att.dic e um arquivo de dados que devem ser utilizados como teste.