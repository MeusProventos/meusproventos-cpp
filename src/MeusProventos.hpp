#pragma once

#include <assert.h>
#include <chrono>
#include <iomanip>
#include <map>
#include <optional>
#include <sstream>
#include <string>

// https://github.com/HowardHinnant/date/
#include <date/date.h>

// https://github.com/vincentlaucsb/csv-parser
// MIT License
#include <vincentlaucsb/csv.hpp>
//

// https://github.com/ben-strasser/fast-cpp-csv-parser
// BSD-3
//#include <ben_strasser/csv.h>
//

// https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
#include <trim/trim.hpp>

using namespace csv; // vincentlaucsb

using tp = std::chrono::system_clock::time_point;

// Linha de Movimentacao
struct MLinha {
  char entradaSaida;        // 'C' ou 'D' (Credito ou Debito)
  tp data;                  // 10/12/2021 (chrono)
  std::string movimentacao; // "Dividendo", "Rendimento", "Amortização", ...
  std::string produto;      // "ABEV3 - AMBEV S/A"
  std::string instituicao;  // "NU INVEST"
  int quantidade;           // 100
  std::optional<long> precoUnitario; // centavos. ex.: 125 para "R$1,25"
  std::optional<long> valorOperacao; // centavos. ex.: 125 para "R$1,25"

  friend std::ostream &operator<<(std::ostream &os, const MLinha &self) {
    //
    os << "MLinha{" << self.entradaSaida << ";" << self.getSData() << ";'"
       << self.movimentacao << "';'" << self.produto << "';'"
       << self.instituicao << "';" << self.quantidade << ";"
       << (self.precoUnitario ? *self.precoUnitario : 0) << ";"
       << (self.valorOperacao ? *self.valorOperacao : 0) << "}";
    return os;
  }

  // data formatada em D/M/A
  std::string getSData() const {
    auto ymd = date::year_month_day{
        date::floor<date::days>(this->data)}; // from time_point
    std::stringstream ssdate;
    ssdate << ymd.day() << "/" << (unsigned)ymd.month() << "/" << ymd.year();
    return ssdate.str();
  }

  std::pair<std::string, int> getCodigoProduto() {
    //
    std::string scod;
    //
    std::stringstream ss(produto);
    ss >> scod;
    //
    assert((scod.length() >= 5) && (scod.length() <= 6));
    //
    char _cod[5];
    int num;
    if (2 == std::sscanf(scod.c_str(), "%4s%d", (char *)_cod, &num))
      return {scod, num};
    else {
      assert(false);
      return {"", -1};
    }
  }
};

struct ItemBens {
  std::string produto;
  int quantidade;
  long valor;
  std::string sdata;

  friend std::ostream &operator<<(std::ostream &os, const ItemBens &self) {
    os << "ItemBens{'" << self.produto << "';" << self.quantidade << ";"
       << self.valor << ";" << self.sdata << "}";
    return os;
  }
};

class MeusProventos {
public:
  static std::vector<MLinha> carregaCSV(std::string arquivo) {

    // Entrada/Sa�da;Data;Movimenta��o;Produto;Institui��o;Quantidade;Pre�o
    // unit�rio;Valor da Opera��o
    //

    CSVReader reader(arquivo);

    std::vector<std::string> cols = reader.get_col_names();
    assert(cols.size() == 8);

    std::vector<MLinha> doc;

    int line_count = 1; // skip header
    for (CSVRow &row : reader) {
      //
      // row["col_name"] = ... // acesso direto a coluna (não utilizado!)
      //
      MLinha mlinha;
      //
      int col = 0;
      for (CSVField &field : row) {
        //
        std::string campo = field.get<>();
        mytrim::trim(campo); // trim already
        //
        switch (col) {
        case 0: {
          if (campo == "Credito")
            mlinha.entradaSaida = 'C';
          else if (campo == "Debito")
            mlinha.entradaSaida = 'D';
          else {
            std::cout << "ERROR! CAMPO 0" << std::endl;
            assert(false);
          }
          break;
        }
        case 1: {
          std::stringstream ss(campo);
          std::tm _tm = {};
          ss >> std::get_time(&_tm, "%d/%m/%Y");
          mlinha.data =
              std::chrono::system_clock::from_time_t(std::mktime(&_tm));
          // mlinha.data = _tm;
          break;
        }
        case 2: {
          mlinha.movimentacao = campo;
          break;
        }
        case 3: {
          std::stringstream sscode(campo);
          std::string code;
          sscode >> code;
          mlinha.produto = code; // campo
          break;
        }
        case 4: {
          mlinha.instituicao = campo;
          break;
        }
        case 5: {
          std::stringstream ss(campo);
          ss >> mlinha.quantidade;
          assert(mlinha.quantidade >= 0);
          break;
        }
        case 6: {
          // limpa pontos
          std::string _campo;
          for (unsigned i = 0; i < campo.length(); i++)
            if (campo[i] != '.')
              _campo += campo[i];
          //
          int real, centavos;
          if (2 == std::sscanf(_campo.c_str(), "R$%d,%d", &real, &centavos)) {
            mlinha.precoUnitario = (long)(100L * real + centavos);
          } else {
            std::string base = "-";
            if (_campo != base) {
              std::cout << "ERRO! precoUnitario = '" << _campo << "'"
                        << std::endl;
              assert(false);
            }
          }
          break;
        }
        case 7: {
          // limpa pontos
          std::string _campo;
          for (unsigned i = 0; i < campo.length(); i++)
            if (campo[i] != '.')
              _campo += campo[i];
          //
          int real, centavos;
          if (2 == std::sscanf(_campo.c_str(), "R$%d,%d", &real, &centavos)) {
            mlinha.valorOperacao = (long)(100L * real + centavos);
            /*
            if (mlinha.quantidade * (*mlinha.precoUnitario) !=
                (*mlinha.valorOperacao)) {
              std::cout << "ERRO! diferença em quantidade x unit =
            valor_operacao"
                        << std::endl;
              std::cout << mlinha.quantidade << " x " << *mlinha.precoUnitario
                        << " == " << *mlinha.valorOperacao << std::endl;
              assert(false);
            }
            */
          } else {
            std::string base = "-";
            if (_campo != base) {
              std::cout << "ERRO! valorOperacao = '" << _campo << "'"
                        << std::endl;
              assert(false);
            }
            assert(!mlinha.precoUnitario);
          }
          break;
        }
        default: {
          std::cout << "ERRO! CAMPO ACIMA DE 8!" << std::endl;
          assert(false);
        }
        };

        /*
        std::cout << " '" << field.get<>() << "'\t";
        if (col == 2)
          if (field.get<>() == "Juros Sobre Capital Pr�prio") {
            std::cout << "COL=2 JUROS OK!" << std::endl;
          }
          */
        col++;
      }
      // std::cout << "LINHA FINALIZADA! mlinha=" << mlinha << std::endl;
      std::cout << "linha #" << line_count << ": " << mlinha << std::endl;
      //
      line_count++;
      //
      doc.push_back(mlinha);
      /*
      if (line_count >= 1) {
        std::cout << "BREAK! Somente uma linha!" << std::endl;
        break;
      }
      */
    }
    //
    return doc;
  }
};
