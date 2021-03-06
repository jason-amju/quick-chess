// -----------------------------------------------------------------------------
// Quick chess - Jason Colman 2016 - just a fun project to keep my hand in.
// -----------------------------------------------------------------------------

#include <assert.h>
#include "eval.h"

int evaluator::calc_score(const board& b, piece_colour pc) const
{
  int total = 0;
  for (auto e : m_evals)
  {
    total += e->calc_score(b, pc) * e->get_weight();
  }
  return total;
}

void evaluator::add(eval* e)
{
  m_evals.push_back(e);
}

static int get_centre_control_score(int i, int j)
{
  int n = i * 8 + j;

  assert(n >= 0);
  assert(n < 64);

  // TODO Make these tunable
  static char SCORES[] = 
  { 
    0, 1, 2,  4,  4,  2,  1, 0,
    1, 2, 4,  8,  8,  4,  2, 1,
    2, 4, 8,  16, 16, 8,  4, 2,
    4, 8, 16, 32, 32, 16, 8, 4,
    4, 8, 16, 32, 32, 16, 8, 4,
    2, 4, 8,  16, 16, 8,  4, 2,
    1, 2, 4,  8,  8,  4,  2, 1,
    0, 1, 2,  4,  4,  2,  1, 0,
  };
  return SCORES[n];
}

int eval_control_centre::calc_score(const board& b, piece_colour pc) const
{
  int total = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      square s = b.get(row_col(i, j));
      if (pc == get_piece_colour(s))
      {
        total += get_centre_control_score(i, j);
      }
      else if (!is_empty(s))
      {
        total -= get_centre_control_score(i, j);
      }
    }
  } 
  return total; 
}
  
eval_material::eval_material()
{
  m_weight = 100;
}

int eval_material::calc_score(const board& b, piece_colour pc) const
{
  // TODO allow these to be tuned?
  int MATERIAL_SCORE[] = { 0, 1, 5, 3, 3, 1000, 9 };

  int total = 0;
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      square s = b.get(row_col(i, j));
      if (pc == get_piece_colour(s))
        total += MATERIAL_SCORE[get_piece_type(s)]; // our piece
      else if (!is_empty(s))
        total -= MATERIAL_SCORE[get_piece_type(s)]; // opponent piece
    }
  } 
  return total; 
}

