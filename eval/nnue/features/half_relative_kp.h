﻿//Definition of input features HalfRelativeKP of NNUE evaluation function

#ifndef _NNUE_FEATURES_HALF_RELATIVE_KP_H_
#define _NNUE_FEATURES_HALF_RELATIVE_KP_H_

#if defined(EVAL_NNUE)

#include "../../../evaluate.h"
#include "features_common.h"

namespace Eval {

	namespace NNUE {

		namespace Features {

			// Feature HalfRelativeKP: Relative position of each piece other than the ball based on own ball or enemy ball
template <Side AssociatedKing>
			class HalfRelativeKP {
			public:
				// feature quantity name
				static constexpr const char* kName = (AssociatedKing == Side::kFriend) ?
					"HalfRelativeKP(Friend)" : "HalfRelativeKP(Enemy)";
				// Hash value embedded in the evaluation function file
				static constexpr std::uint32_t kHashValue =
					0xF9180919u ^ (AssociatedKing == Side::kFriend);
				// Piece type excluding balls
				static constexpr IndexType kNumPieceKinds = (fe_end - fe_hand_end) / SQUARE_NB;
				// width of the virtual board with the ball in the center
				static constexpr IndexType kBoardWidth = FILE_NB * 2 - 1;
				// height of a virtual board with balls in the center
				static constexpr IndexType kBoardHeight = RANK_NB * 2 - 1;
				// number of feature dimensions
				static constexpr IndexType kDimensions =
					kNumPieceKinds * kBoardHeight * kBoardWidth;
				// The maximum value of the number of indexes whose values ​​are 1 at the same time among the feature values
				static constexpr IndexType kMaxActiveDimensions = PIECE_NUMBER_KING;
				// Timing of full calculation instead of difference calculation
				static constexpr TriggerEvent kRefreshTrigger =
					(AssociatedKing == Side::kFriend) ?
					TriggerEvent::kFriendKingMoved : TriggerEvent::kEnemyKingMoved;

				// Get a list of indices with a value of 1 among the features
				static void AppendActiveIndices(const Position& pos, Color perspective,
					IndexList* active);

				// Get a list of indexes whose values ​​have changed from the previous one among the feature quantities
				static void AppendChangedIndices(const Position& pos, Color perspective,
					IndexList* removed, IndexList* added);

				// Find the index of the feature quantity from the ball position and BonaPiece
				static IndexType MakeIndex(Square sq_k, BonaPiece p);

			private:
				// Get the piece information
				static void GetPieces(const Position& pos, Color perspective,
					BonaPiece** pieces, Square* sq_target_k);
			};

		} // namespace Features

	} // namespace NNUE

} // namespace Eval

#endif // defined(EVAL_NNUE)

#endif