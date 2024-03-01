import React, {
	useCallback,
	useContext,
	useEffect,
	useMemo,
	useState,
} from 'react';
import Select from 'react-select';
import { Alert, Button, Form } from 'react-bootstrap';
import { useTranslation } from 'react-i18next';
import zip from 'lodash/zip';

import { AppContext } from '../Contexts/AppContext';
import Section from '../Components/Section';

import { BUTTON_MASKS, DPAD_MASKS, getButtonLabels } from '../Data/Buttons';
import useMultiPinStore, { MaskPayload } from '../Store/useMultiPinStore';

const MASK_OPTIONS = [
	...BUTTON_MASKS.map((mask) => ({ ...mask, type: 'customButtonMask' })),
	...DPAD_MASKS.map((mask) => ({ ...mask, type: 'customDpadMask' })),
];
type PinCell = [string, MaskPayload];
type PinRow = [PinCell, PinCell];
type PinList = [PinRow];

export default function MultiMappingPage() {
	const { fetchPins, pins, savePins, setPinMasks } = useMultiPinStore();
	const { updateUsedPins } = useContext(AppContext);
	const [saveMessage, setSaveMessage] = useState('');

	const { t } = useTranslation('');

	useEffect(() => {
		fetchPins();
	}, []);

	const handleSubmit = async (e) => {
		e.preventDefault();
		e.stopPropagation();
		try {
			await savePins();
			updateUsedPins();
			setSaveMessage(t('Common:saved-success-message'));
		} catch (error) {
			setSaveMessage(t('Common:saved-error-message'));
		}
	};

	const pinList = useMemo<PinList>(() => {
		const pinArray = Object.entries(pins);
		return zip(
			pinArray.slice(0, pinArray.length / 2),
			pinArray.slice(pinArray.length / 2, pinArray.length),
		);
	}, [pins]);

	const createCell = useCallback(
		([pin, pinValue]: PinCell) => (
			<div className="d-flex col py-2">
				<div
					className="d-flex align-items-center flex-shrink-0"
					style={{ width: '4rem' }}
				>
					<label htmlFor={pin}>{pin.toUpperCase()}</label>
				</div>
				<Select
					inputId={pin}
					className="text-primary flex-grow-1"
					isClearable
					isSearchable
					options={MASK_OPTIONS}
					placeholder={t('MultiMapping:placeholder')}
					value={MASK_OPTIONS.filter(
						({ value, type }) =>
							(pinValue.customButtonMask & value &&
								type === 'customButtonMask') ||
							(pinValue.customDpadMask & value && type === 'customDpadMask'),
					)}
					getOptionValue={(option) => `${option.type}.${option.value}`}
					isMulti
					onChange={(selected) =>
						setPinMasks(
							pin,
							selected.reduce(
								(masks, option) => ({
									customButtonMask:
										option.type === 'customButtonMask'
											? masks.customButtonMask ^ option.value
											: masks.customButtonMask,
									customDpadMask:
										option.type === 'customDpadMask'
											? masks.customDpadMask ^ option.value
											: masks.customDpadMask,
								}),
								{ customButtonMask: 0, customDpadMask: 0 },
							),
						)
					}
				/>
			</div>
		),
		[],
	);

	return (
		<>
			<Section title={t('MultiMapping:header-text')}>
				<Form onSubmit={handleSubmit}>
					<div className="gx-3">
						{pinList.map(([cell1, cell2], i) => (
							<div key={`pin-row-${i}`} className="row">
								{createCell(cell1)}
								{createCell(cell2)}
							</div>
						))}
					</div>
					<Button type="submit" className="my-4">
						{t('Common:button-save-label')}
					</Button>
					{saveMessage && <Alert variant="secondary">{saveMessage}</Alert>}
				</Form>
			</Section>
		</>
	);
}
