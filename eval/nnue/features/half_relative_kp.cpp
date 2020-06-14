﻿//Definition of input features HalfRelativeKP of NNUE evaluation function

#if defined(EVAL_NNUE)

#include "half_relative_kp.h"
#include "index_list.h"

namespace Eval::NNUE::Features
{

	// Find the index of the feature quantity from the ball position and BonaPiece
	template <Side AssociatedKing>
	IndexType HalfRelativeKP<AssociatedKing>::MakeIndex(
		const Square sq_k, const BonaPiece p) {
		constexpr auto W = kBoardWidth;
		constexpr auto H = kBoardHeight;
		const IndexType piece_index = (p - fe_hand_end) / SQUARE_NB;
		const auto sq_p = static_cast<Square>((p - fe_hand_end) % SQUARE_NB);
		const auto relative_file = file_of(sq_p) - file_of(sq_k) + W / 2;
		const auto relative_rank = rank_of(sq_p) - rank_of(sq_k) + H / 2;
		return H * W * piece_index + H * relative_file + relative_rank;
	}

	// Get the piece information
	template <Side AssociatedKing>
	void HalfRelativeKP<AssociatedKing>::GetPieces(
		const Position& pos, const Color perspective,
		BonaPiece** pieces, Square* sq_target_k) {
		*pieces = perspective == BLACK ?
			pos.eval_list()->piece_list_fb() :
			pos.eval_list()->piece_list_fw();
		const auto target = AssociatedKing == Side::kFriend ?
			                    static_cast<PieceNumber>(PIECE_NUMBER_KING + perspective) :
			                    static_cast<PieceNumber>(PIECE_NUMBER_KING + ~perspective);
		*sq_target_k = static_cast<Square>(((*pieces)[target] - f_king) % SQUARE_NB);
	}

	// Get a list of indices with a value of 1 among the features
	template <Side AssociatedKing>
	void HalfRelativeKP<AssociatedKing>::AppendActiveIndices(
		const Position& pos, const Color perspective, IndexList* active) {
		// do nothing if array size is small to avoid compiler warning
		if (RawFeatures::kMaxActiveDimensions < kMaxActiveDimensions) return;

		BonaPiece* pieces;
		Square sq_target_k;
		GetPieces(pos, perspective, &pieces, &sq_target_k);
		for (auto i = PIECE_NUMBER_ZERO; i < PIECE_NUMBER_KING; ++i) {
			if (pieces[i] >= fe_hand_end) {
				if (pieces[i] != BONA_PIECE_ZERO) {
					active->push_back(MakeIndex(sq_target_k, pieces[i]));
				}
			}
		}
	}

	// Get a list of indexes whose values ​​have changed from the previous one among the feature quantities
	template <Side AssociatedKing>
	void HalfRelativeKP<AssociatedKing>::AppendChangedIndices(
		const Position& pos, const Color perspective,
		IndexList* removed, IndexList* added) {
		BonaPiece* pieces;
		Square sq_target_k;
		GetPieces(pos, perspective, &pieces, &sq_target_k);
		const auto& dp = pos.state()->dirtyPiece;
		for (auto i = 0; i < dp.dirty_num; ++i) {
			if (dp.pieceNo[i] >= PIECE_NUMBER_KING) continue;
			const auto old_p = static_cast<BonaPiece>(
				dp.changed_piece[i].old_piece.from[perspective]);
			if (old_p >= fe_hand_end) {
				if (old_p != BONA_PIECE_ZERO) {
					removed->push_back(MakeIndex(sq_target_k, old_p));
				}
			}
			const auto new_p = static_cast<BonaPiece>(
				dp.changed_piece[i].new_piece.from[perspective]);
			if (new_p >= fe_hand_end) {
				if (new_p != BONA_PIECE_ZERO) {
					added->push_back(MakeIndex(sq_target_k, new_p));
				}
			}
		}
	}

	template class HalfRelativeKP<Side::kFriend>;
	template class HalfRelativeKP<Side::kEnemy>;
} // namespace Eval

#endif // defined(EVAL_NNUE)