import { create } from 'zustand';

import WebApi from '../Services/WebApi';
import { BUTTON_ACTIONS } from '../Data/Pins';

export type MaskPayload = {
	action: number;
	customButtonMask: number;
	customDpadMask: number;
};
type State = {
	pins: { [key: string]: MaskPayload };
	loadingPins: boolean;
};

type Actions = {
	fetchPins: () => void;
	setPinMask: (pin: string, customButtonMask: number) => void;
	savePins: () => Promise<object>;
};
const DEFAULT_MASKS_STATE = {
	action: 0,
	customButtonMask: 0,
	customDpadMask: 0,
};
const INITIAL_STATE: State = {
	pins: {
		pin00: DEFAULT_MASKS_STATE,
		pin01: DEFAULT_MASKS_STATE,
		pin02: DEFAULT_MASKS_STATE,
		pin03: DEFAULT_MASKS_STATE,
		pin04: DEFAULT_MASKS_STATE,
		pin05: DEFAULT_MASKS_STATE,
		pin06: DEFAULT_MASKS_STATE,
		pin07: DEFAULT_MASKS_STATE,
		pin08: DEFAULT_MASKS_STATE,
		pin09: DEFAULT_MASKS_STATE,
		pin10: DEFAULT_MASKS_STATE,
		pin11: DEFAULT_MASKS_STATE,
		pin12: DEFAULT_MASKS_STATE,
		pin13: DEFAULT_MASKS_STATE,
		pin14: DEFAULT_MASKS_STATE,
		pin15: DEFAULT_MASKS_STATE,
		pin16: DEFAULT_MASKS_STATE,
		pin17: DEFAULT_MASKS_STATE,
		pin18: DEFAULT_MASKS_STATE,
		pin19: DEFAULT_MASKS_STATE,
		pin20: DEFAULT_MASKS_STATE,
		pin21: DEFAULT_MASKS_STATE,
		pin22: DEFAULT_MASKS_STATE,
		pin23: DEFAULT_MASKS_STATE,
		pin24: DEFAULT_MASKS_STATE,
		pin25: DEFAULT_MASKS_STATE,
		pin26: DEFAULT_MASKS_STATE,
		pin27: DEFAULT_MASKS_STATE,
		pin28: DEFAULT_MASKS_STATE,
		pin29: DEFAULT_MASKS_STATE,
	},
	loadingPins: false,
};

const useMultiPinStore = create<State & Actions>()((set, get) => ({
	...INITIAL_STATE,
	fetchPins: async () => {
		set({ loadingPins: true });
		const pins = await WebApi.getPinMappingsV2();
		set((state) => ({
			...state,
			pins,
			loadingPins: false,
		}));
	},
	setPinMask: (pin, customButtonMask) =>
		set((state) => ({
			...state,
			pins: {
				...state.pins,
				[pin]: {
					action: customButtonMask ? BUTTON_ACTIONS.CUSTOM_BUTTON_COMBO : 0,
					customButtonMask,
					customDpadMask: 0,
				},
			},
		})),
	savePins: async () => WebApi.setPinMappingsV2(get().pins),
}));

export default useMultiPinStore;