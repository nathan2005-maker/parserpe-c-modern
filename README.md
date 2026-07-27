# parserpe-c-modern

`parserpe-c-modern` é um transpilador C minimalista e SDK de análise de baixo nível. Ele lê arquivos-fonte escritos em uma sintaxe alternativa no estilo Lisp (apenas cadeias de chamadas de função) e emite código C simples e compilável (`a.c`).

Além disso, empacota módulos de baixo nível reutilizáveis — um leitor de cabeçalho PE e um desmontador x86-64 baseado em Zydis — atualmente em desenvolvimento inicial.

Ele oferece:

- Um transpilador dead-simple: chamadas de função e literais de string → `main` C padrão
- Compilação automática do `a.c` gerado em um executável via invocação embutida do `gcc`
- Desmontagem automática da seção `.text` resultante em assembly x86-64 legível (`a.asm`), quando a seção é encontrada
- Um SDK pequeno e reutilizável (`sdk.h`) com `String_View`, arrays dinâmicos, parsing de PE e desmontagem baseada em Zydis

## Usage

```
./readpe <file>
```

O programa lê `<file>`, transpila o conteúdo e escreve o resultado em `a.c` no diretório atual. Em seguida compila `a.c` para `a.exe` e desmonta a seção `.text` para `a.asm` (se a seção existir).

## Input syntax

O parser reconhece apenas dois tipos de expressão: chamadas de função (`identificador(argumentos...)`) e literais de string (`"..."`). Argumentos de uma chamada podem ser strings ou outras chamadas de função (aninhadas). Cada chamada de nível superior vira um statement dentro do `main` gerado automaticamente.

Input (`demo.txt`):

```
printf("Hello, GitHub!\n")
printf("Language C CXX\n")
```

Generated output (`a.c`):

```c
#include <stdio.h>

int main(int argc, char *argv[]) {
	printf("Hello, GitHub!\n");
	printf("Language C CXX\n");
	return 0;
}
```

> Nota: o tokenizer **não interpreta sequências de escape** — o `\n` dentro das aspas é copiado literalmente (dois caracteres, `\` e `n`) para o `a.c` gerado. É o `gcc`, ao compilar `a.c`, quem de fato interpreta esse `\n` como quebra de linha.

Não há validação semântica — qualquer identificador seguido de `(...)` é aceito e traduzido literalmente. É responsabilidade do `a.c` gerado compilar (ex.: `printf` requer `<stdio.h>`, que o gerador de código já injeta automaticamente).

## Build from source

```
cmake -S . -B build
cmake --build build
```

O binário resultante é `build/readpe.exe`. **O build é exclusivamente para Windows** (veja "Known Problems" abaixo) — não há suporte a cross-compilação ou build nativo em Linux/macOS.

## Run the full pipeline

```
./readpe demo.txt   # gera a.c, compila a.exe, escreve a.asm
gcc a.c -o demo     # opcional: compilação manual
./demo              # Hello, GitHub! / Language C CXX
```

## Options

### `<file>`

- Type: string
- Default: —
- Required: yes

Caminho para o arquivo-fonte escrito na sintaxe Lisp-like. O arquivo é tokenizado, parseado em uma AST e então traduzido para C.

```
./readpe myscript.txt
```

## Outputs

### `a.c`

O arquivo-fonte C gerado. Sempre escrito no diretório de trabalho atual. Contém `#include <stdio.h>`, um `int main(...)` e um statement por chamada de função de nível superior encontrada no input.

### `a.exe`

O executável compilado automaticamente pela ferramenta via `gcc --std=c17 -o a.exe a.c`. Se a compilação falhar, o programa encerra com código 1.

### `a.asm`

A desmontagem da seção `.text` de `a.exe`, escrita em sintaxe Intel, gerada usando o desmontador baseado em Zydis embutido. **Só é gerada se a seção `.text` for encontrada** no executável. Bytes inválidos são pulados um a um com um marcador de erro em vez de interromper toda a desmontagem.

## SDK Modules

Estes headers estão disponíveis para reuso ao incluir `sdk.h`:

| Module | Responsibility |
|---|---|
| `token.h` | Tokenizer: quebra o source em tokens (`(`, `)`, identificadores, strings) com rastreamento de linha/coluna para mensagens de erro. |
| `ast.h` | Parser recursive-descent: consome tokens e constrói a AST (`AST_STRING`, `AST_FUNCTION_CALL`, `AST_STATEMENTS`). |
| `cg.h` | Gerador de código: percorre a AST e escreve o `a.c` final (includes + main). |
| `sv.h` | `String_View` — par ponteiro+tamanho para strings zero-copy, usado em todo o projeto. |
| `da.h` | Macro `APPEND` para arrays dinâmicos genéricos (usado por `Tokens` e `ASTs`). |
| `io.h` | Lê um arquivo inteiro para um buffer no heap, devolvido como `String_View`. |
| `VTableError.h` | Tipo de erro compacto baseado em tagged-union para relato padronizado de falhas. |
| `pe_parser.h` | Leitor de cabeçalho PE (validação de DOS header, busca de seção). *(em desenvolvimento)* |
| `disasm.h` | Desmontador x86-64 via Zydis. *(em desenvolvimento — interface definida, implementação incompleta)* |

## Dependencies

Gerenciadas via CMake `FetchContent` (baixadas automaticamente no build; sem instalação manual necessária):

- `spdlog v1.14.1` — logging
- `zydis v4.1.0` — decodificação de instruções x86-64 (usado por `disasm.h`)

## Known Problems

### Windows-only build

O projeto inclui `<windows.h>` (via `pch.h`), incondicionalmente, para o parser PE e utilitários de processo. Portanto, o build atual é específico para Windows. Cross-compilação ou builds nativos em Linux/macOS vão falhar porque `IMAGE_DOS_HEADER`, `IMAGE_NT_HEADERS` e outros tipos do Windows são exigidos em tempo de compilação.

**Workaround:** compile no Windows com MinGW-w64, MSYS2 ou MSVC. Um port futuro exigiria abstrair os tipos específicos do Windows atrás de compilação condicional por plataforma.

### No semantic validation

O transpilador aceita qualquer identificador como nome de função e qualquer string como argumento. Se o `a.c` gerado referenciar uma função indefinida ou um header ausente, o `gcc` vai falhar durante a etapa automática de compilação.

**Workaround:** garanta que todas as funções usadas no input estejam declaradas em headers C padrão (ou forneça seus próprios includes em um fork customizado do gerador de código).

### Disassembler skips invalid bytes

Quando o Zydis não consegue decodificar uma instrução, o desmontador cai para imprimir um único byte inválido e avança em um. Isso evita crashes em dados embutidos em `.text`, mas pode produzir saída enganosa se o stream de bytes estiver genuinamente corrompido.
