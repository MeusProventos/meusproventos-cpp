#include "MeusProventos.hpp"

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout
        << "ERRO! 'meusproventos' espera uma entrada. Veja README. Exemplo:"
        << std::endl;
    std::cout << "./meusproventos --entrada exemplos/exemplo0.csv" << std::endl;
    return 1;
  }

  std::string arg_entrada = argv[1];
  std::string arquivo_csv = argv[2];

  assert(arg_entrada == "--entrada");
  assert(arquivo_csv.length() >= 4); // TODO: checar se é .csv

  auto doc = MeusProventos::carregaCSV(arquivo_csv);

  std::cout << "#linhas = " << doc.size() << std::endl;

  // =======================

  std::cout << std::endl;
  std::cout << "PASSO 2: ORDENANDO POR DATA" << std::endl;
  std::cout << std::endl;

  std::sort(
      doc.begin(), doc.end(),
      [](const MLinha &a, const MLinha &b) -> bool { return a.data < b.data; });

  // =======================

  std::cout << std::endl;
  std::cout << "PASSO 3: IMPRIMINDO TUDO ORDENADO" << std::endl;
  std::cout << std::endl;

  for (unsigned k = 0; k < doc.size(); k++)
    std::cout << "linha #" << (k + 1) << ": " << doc[k] << std::endl;

  std::cout << "#linhas = " << doc.size() << std::endl;

  // =======================

  std::cout << std::endl;
  std::cout << "PASSO 4: CONTABILIZANDO BENS" << std::endl;
  std::cout << std::endl;

  std::map<std::string, ItemBens> bens;

  // =======================

  std::cout << std::endl;
  std::cout << " => IPO MANUAL" << std::endl;
  std::cout << std::endl;

  std::string str_ipo = "EEFF3";

  bens[str_ipo] = ItemBens{
      .produto = str_ipo, .quantidade = 1, .valor = 100, .sdata = "01/05/2021"};

  // =======================

  for (unsigned k = 0; k < doc.size(); k++) {

    if ((doc[k].movimentacao == "Transfer�ncia - Liquida��o") ||
        (doc[k].movimentacao == "Transferência - Liquidação")) {

      if (bens.find(doc[k].produto) == bens.end()) {
        // not found
        bens[doc[k].produto] = ItemBens{.produto = doc[k].produto,
                                        .quantidade = doc[k].quantidade,
                                        .valor = *doc[k].valorOperacao,
                                        .sdata = doc[k].getSData()};

        std::cout << "Encontrou produto '" << doc[k].produto << "':\n\t=> "
                  << bens[doc[k].produto] << std::endl;
      } else {
        // found
        std::cout << "ERRO! Dupla 'Transferência - Liquidação' nao prevista..."
                  << std::endl;
        std::cout << "Produto = '" << doc[k].produto << "'" << std::endl;
        assert(false);
      }
    }
  }

  // =======================

  std::cout << std::endl;
  std::cout << "PASSO 5: CONTABILIZANDO PROVENTOS" << std::endl;
  std::cout << std::endl;

  std::map<std::string, long> proventos;

  for (auto &x : bens)
    proventos[x.first] = 0;

  for (unsigned k = 0; k < doc.size(); k++) {
    if ((doc[k].movimentacao == "Rendimento") ||
        (doc[k].movimentacao == "Amortiza��o") ||
        (doc[k].movimentacao == "Amortização") ||
        (doc[k].movimentacao == "Dividendo")) {
      //
      std::cout << " => PROVENTO TIPO '" << doc[k].movimentacao
                << " VALOR=" << *doc[k].valorOperacao << " ' PARA '"
                << doc[k].produto << "'" << std::endl;
      //
      proventos[doc[k].produto] += *doc[k].valorOperacao;
      std::cout << "    ~> proventos '" << doc[k].produto
                << "' = " << proventos[doc[k].produto] << std::endl;
      //
    }
  }

  // =======================

  std::cout << std::endl;
  std::cout << "PASSO 6: ORDENANDO BENS" << std::endl;
  std::cout << std::endl;

  std::vector<ItemBens> vbens;
  for (auto &x : bens)
    vbens.push_back(x.second);

  std::sort(vbens.begin(), vbens.end(),
            [](const ItemBens &a, const ItemBens &b) -> bool {
              return a.produto < b.produto;
            });

  std::cout << std::endl;
  std::cout << "------------------------------------------------" << std::endl;
  std::cout << "                Bens e Direitos                 " << std::endl;
  std::cout << "------------------------------------------------" << std::endl;
  std::cout << std::endl;

  std::cout << "#\tQtd\tValor\tData\tProventos\tProduto" << std::endl;
  for (unsigned b = 0; b < vbens.size(); b++) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2)
       << (double)(vbens[b].valor / 100.0);
    //
    std::stringstream ssprov;
    ssprov << std::fixed << std::setprecision(2)
           << (double)(proventos[vbens[b].produto] / 100.0);
    // ssprov << proventos[vbens[b].produto];
    //
    std::cout << "#" << (b + 1) << "\t" << vbens[b].quantidade << "\t"
              << ss.str() << "\t" << vbens[b].sdata << "\t" << ssprov.str()
              << "\t'" << vbens[b].produto << "'" << std::endl;
  }

  std::cout << std::endl;
  std::cout << std::endl;

  // ==================

  return 0;
}