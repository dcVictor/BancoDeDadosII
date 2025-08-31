Implementar um programa em C que imprima os dados do arquivo customer.dat (Baixar arquivo)

Esse arquivo armazena nos bytes iniciais o esquema dos dados (cabeçalho). O cabeçalho esta ogranizado da seguinte forma:

- Primeiros 15 bytes representam o nome do atributo (string)

- O byte seguinte o tipo (char), que pode ser S para string, C para char, I para int e F para float

- Depois um inteiro que armazenad o tamanho do atributo. Para os atributos numéricos, foi utilizado o sizeof deles para o tamanho.

** cuidado: o arquivo foi gerado no Linux, talvez o sizeof de outros SO possa retornar tamanhos diferentes

O cabeçalho terá uma sequência de atributos (no máximo 10). Caso existirem menos de 10, o nome do atributo que delimita é #, ou seja, basta testar nome[0]!='#' para saber que a sequência do atributo é válida.

O programa devera ser o customer.dat (Baixar arquivo) é gerar um relatório no formato:

att1       att2    att3   att4  ....  attn

val1     val2    val3   val4  ....  valn

Para esta atividade, crie apenas um programa C. o nome dele deve ser a matrícula do estudante.